#include "log/log.hpp"
#include <filesystem>
#include <iomanip>
#include <iostream>

Log::Log(const std::string& filename, bool enabled, const FlushPolicy& policy)
    : filename_(filename), enabled_(enabled), policy_(policy), update_count_(0),
      last_flush_time_(std::chrono::steady_clock::now()) {
    if (enabled_) {
        file_.open(filename_, std::ios::app);
        if (!file_.is_open()) {
            std::cerr << "Failed to open log file: " << filename_ << std::endl;
        }
    }
}

Log::~Log() {
    if (enabled_ && file_.is_open()) {
        flush();
        file_.close();
    }
}

void Log::log(const std::shared_ptr<LogEntry>& entry) {
    if (!enabled_ || !file_.is_open()) return;

    std::lock_guard<std::mutex> lock(mutex_);
    buffer_.emplace_back(entry, entry->timestamp());
    update_count_++;

    auto elapsed = std::chrono::duration_cast<std::chrono::minutes>(
        std::chrono::steady_clock::now() - last_flush_time_).count();
    if (update_count_ >= policy_.max_updates || elapsed >= policy_.max_minutes) {
        flush();
        update_count_ = 0;
        last_flush_time_ = std::chrono::steady_clock::now();
    }
}

void Log::flush() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (buffer_.empty()) return;

    check_size_and_truncate();

//    std::sort(buffer_.begin(), buffer_.end(),
//              [](const auto& a, const auto& b) { return a.second < b.second; });

    for (const auto& [entry, timestamp] : buffer_) {
        file_ << entry->format() << std::endl;
    }
    file_.flush();
    buffer_.clear();
}

void Log::truncate() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (enabled_ && file_.is_open()) {
        file_.close();
    }
    std::ofstream ofs(filename_, std::ios::trunc);
    ofs << "Log truncated at start of new session\n";
    ofs.close();
    if (enabled_) {
        file_.open(filename_, std::ios::app);
        if (!file_.is_open()) {
            std::cerr << "Failed to reopen log file after truncation: " << filename_ << std::endl;
        }
    }
}

void Log::check_size_and_truncate() {
    if (std::filesystem::exists(filename_) && std::filesystem::file_size(filename_) > policy_.max_size) {
        file_.close();
        std::ofstream ofs(filename_, std::ios::trunc);
        ofs << "Log truncated due to size limit\n";
        ofs.close();
        file_.open(filename_, std::ios::app);
    }
}
