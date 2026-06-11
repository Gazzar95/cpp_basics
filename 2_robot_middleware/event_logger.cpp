
// `EventLogger` class in `event_logger.h`/`.cpp`:
// - Constructor takes a `std::filesystem::path` for the log file
// - Uses C++17 `std::filesystem` to create the log directory if it doesn't exist
// - `log(const std::string& source, const std::string& message)` — writes a timestamped entry to
// file
// - `flush()` — ensures all buffered writes hit disk
// - `readBack(int n)` — reads and prints the last n lines from the log file
// - `rotateLogs(int max_files)` — if more than `max_files` log files exist in the directory, delete
// the oldest

#pragma once
#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct LogRecord {
  uint32_t timestamp;
  uint8_t severity;
  char message[64];
};

class EventLogger {
public:
  // Constructor - creates dir from input path if one doesn't exist
  EventLogger(const std::filesystem::path& file) : file_path{file} {
    std::filesystem::create_directories(file_path.parent_path());
  }

  // Log - creats file with message and updates file if it already exists
  void log(const std::string& source, const std::string& message) {
    time++;

    std::ofstream out(file_path, std::ios::app);
    out << "[" << time << "] " << source << ": " << message << "\n";
  }

  // flush - pushes log off the drive
  void flush() {
    std::ofstream out(file_path, std::ios::app);
    out.flush();
  }

  void readback(int n) {
    std::ifstream in(file_path);

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(in, line)) {
      lines.push_back(line);
    };

    int start = lines.size() - n;
    if (start < 0) start = 0;

    for (int i = start; i < lines.size(); i++) {
      std::cout << lines[i] << "\n";
    }
  }

  void rotateLogs(int max_files) {
    std::vector<std::filesystem::path> entries;

    for (const auto& entry : std::filesystem::directory_iterator(file_path.parent_path())) {
      entries.push_back(entry.path());
    }

    while (entries.size() > max_files) {
      auto oldest_path = entries[0];
      auto oldest_time = std::filesystem::last_write_time(entries[0]);

      for (const auto& entry_path : entries) {
        auto t = std::filesystem::last_write_time(entry_path);
        if (t < oldest_time) {
          oldest_time = t;
          oldest_path = entry_path;
        }
      }

      std::filesystem::remove(oldest_path);

      entries.erase(std::remove(entries.begin(), entries.end(), oldest_path), entries.end());
    }
  }

  void logBinary(uint32_t timestamp, uint8_t severity, const std::string& message) {
    LogRecord record;
    record.timestamp = timestamp;
    record.severity = severity;
    std::strncpy(record.message, message.c_str(), 63);
    record.message[63] = '\0';

    std::filesystem::path bin_path = file_path;
    bin_path.replace_extension(".bin");

    std::ofstream out(bin_path, std::ios::binary | std::ios::app);
    out.write(reinterpret_cast<const char*>(&record), sizeof(record));
  }

  void parseBinaryLog(const std::filesystem::path& file) {
    std::ifstream in(file, std::ios::binary);

    LogRecord log;
    while (in.read(reinterpret_cast<char*>(&log), sizeof(log))) {
      std::cout << log.message << " | " << log.timestamp << "\n";
    }
  }
}

private : std::filesystem::path file_path;
int time{};
}
;