#include "tcp/connection.hpp"
#include "tcp/ip_tcp_header.hpp"
#include <netinet/tcp.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>

ConnectionKey::ConnectionKey(const std::string& src_ip_, uint16_t src_port_, const std::string& dst_ip_, uint16_t dst_port_)
    : src_ip(src_ip_), src_port(src_port_), dst_ip(dst_ip_), dst_port(dst_port_) {}


bool ConnectionKey::operator<(const ConnectionKey& other) const {
    return (src_ip + std::to_string(src_port) + dst_ip + std::to_string(dst_port)) <
           (other.src_ip + std::to_string(other.src_port) + other.dst_ip + std::to_string(other.dst_port));
}

bool ConnectionKey::operator==(const ConnectionKey& other) const {
    return (src_ip == other.src_ip && src_port == other.src_port &&
           dst_ip == other.dst_ip && dst_port == other.dst_port) ||
           (src_ip == other.dst_ip && src_port == other.dst_port &&
           dst_ip == other.src_ip && dst_port == other.src_port);
}

Connection::Connection(const ConnectionKey& key, int id)
    : key_(key), id_(id), is_client_initiated_(false), last_update_(std::chrono::steady_clock::now()) {
}

tcp_state determine_new_client_state(tcp_state current, uint8_t flags) {
    if (flags & TH_RST) return tcp_state::closed;
    switch (current) {
        case tcp_state::closed:
            if (flags & TH_SYN && !(flags & TH_ACK)) return tcp_state::syn_sent;
            break;
        case tcp_state::syn_sent:
            if ((flags & TH_SYN) && (flags & TH_ACK)) return tcp_state::syn_received;
            break;
        case tcp_state::syn_received:
            if (flags & TH_ACK && !(flags & TH_SYN)) return tcp_state::established;
            break;
        case tcp_state::established:
            if (flags & TH_FIN) return tcp_state::fin_wait_1;
            break;
        case tcp_state::fin_wait_1:
            if (flags & TH_ACK && !(flags & TH_FIN)) return tcp_state::fin_wait_2;
            if (flags & TH_FIN) return tcp_state::time_wait;  // Simultaneous close
            break;
        case tcp_state::fin_wait_2:
            if (flags & TH_FIN) return tcp_state::time_wait;
            break;
        case tcp_state::time_wait:
            if (flags & TH_ACK) return tcp_state::closed;
            break;
        default:
            break;
    }
    return current;
}

tcp_state determine_new_server_state(tcp_state current, uint8_t flags) {
    if (flags & TH_RST) return tcp_state::closed;
    switch (current) {
        case tcp_state::closed:
            if (flags & TH_SYN && !(flags & TH_ACK)) return tcp_state::syn_received;
            break;
        case tcp_state::syn_received:
            if ((flags & (TH_SYN | TH_ACK)) == (TH_SYN | TH_ACK)) return tcp_state::syn_received;  // Server sends SYN-ACK
            if (flags & TH_ACK && !(flags & TH_SYN)) return tcp_state::established;  // Client ACK
            break;
        case tcp_state::established:
            if (flags & TH_FIN) return tcp_state::close_wait;
            break;
        case tcp_state::close_wait:
            if (flags & TH_FIN) return tcp_state::last_ack;
            break;
        case tcp_state::last_ack:
            if (flags & TH_ACK) return tcp_state::closed;
            break;
        default:
            break;
    }
    return current;
}

void Connection::update_client_state(uint8_t flags) {
    tcp_state new_state = determine_new_client_state(client_state_.state, flags);
    if (new_state != client_state_.state) {
        auto timestamp = std::chrono::steady_clock::now();
		client_state_.state = new_state;
        client_state_.start_time = timestamp;
        last_update_ = timestamp;
    }
}

void Connection::update_server_state(uint8_t flags) {
    tcp_state new_state = determine_new_server_state(server_state_.state, flags);
    if (new_state != server_state_.state) {
        auto timestamp = std::chrono::steady_clock::now();
        server_state_.state = new_state;
        server_state_.start_time = timestamp;
        last_update_ = timestamp;
    }
}

void Connection::update_state(const ConnectionKey& key , uint8_t flags) {
    if (flags & TH_RST) {
        update_client_state(flags);
        update_server_state(flags);
        return;
    }

    if (key.src_ip == client_ip_) {
        update_client_state(flags);
        // Update server based on client actions
        if (client_state_.state == tcp_state::syn_sent && (flags & TH_SYN) && !(flags & TH_ACK)) {
            // Client SYN, server will respond with SYN-ACK (handled in !is_client_packet)
        } else if (client_state_.state == tcp_state::syn_received && (flags & TH_ACK) && !(flags & TH_SYN)) {
            update_server_state(flags);  // Client ACK completes handshake
        } else if (client_state_.state == tcp_state::established && (flags & TH_FIN)) {
            update_server_state(flags);  // Client FIN, server to close_wait
        } else if (client_state_.state == tcp_state::time_wait && (flags & TH_ACK)) {
            update_server_state(flags);  // Final ACK
        }
    } else {
        update_server_state(flags);
        // Update client based on server actions
        if (server_state_.state == tcp_state::syn_received && (flags & (TH_SYN | TH_ACK)) == (TH_SYN | TH_ACK)) {
            update_client_state(flags) ;  // Server SYN-ACK
        } else if (server_state_.state == tcp_state::established && (flags & TH_FIN)) {
            update_client_state(flags);  // Server FIN
        } else if (server_state_.state == tcp_state::last_ack && (flags & TH_ACK)) {
            update_client_state(flags);  // Server ACK (unlikely, but symmetric)
        }
    }
}

bool Connection::should_clean_up() const {
    auto now = std::chrono::steady_clock::now();
    if (client_state_.state == tcp_state::closed && server_state_.state == tcp_state::closed) return true;
    if (client_state_.state == tcp_state::time_wait || server_state_.state == tcp_state::time_wait) {
        return std::chrono::duration_cast<std::chrono::seconds>(now - last_update_) >= TIME_WAIT_DURATION;
    }
    return false;
}

std::string Connection::get_current_state(std::chrono::steady_clock::time_point now, tcp_state prev_client_state, tcp_state prev_server_state) const {
    std::ostringstream oss;
    oss << "client:" << state_to_string(client_state_.state);
    if (prev_client_state == tcp_state::established && client_state_.state != tcp_state::established) {
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(now - client_state_.start_time).count();
        double duration_s = duration_us / 1000000.0;
        oss << "(" << std::fixed << std::setprecision(3) << duration_s << " s)";
    }
    oss << " server:" << state_to_string(server_state_.state);
    if (prev_server_state == tcp_state::established && server_state_.state != tcp_state::established) {
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(now - server_state_.start_time).count(); //todo
        double duration_s = duration_us / 1000000.0;
        oss << "(" << std::fixed << std::setprecision(3) << duration_s << " s)";
    }
    return oss.str();
}

void Connection::initiate_client(const std::string& src_ip)
{
	client_ip_ = src_ip;
	is_client_initiated_ = true;
}

bool Connection::is_client_initiated()
{
	return is_client_initiated_;
}
