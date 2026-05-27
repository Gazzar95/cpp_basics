
// `EventLogger` class in `event_logger.h`/`.cpp`:
// - Constructor takes a `std::filesystem::path` for the log file
// - Uses C++17 `std::filesystem` to create the log directory if it doesn't exist
// - `log(const std::string& source, const std::string& message)` — writes a timestamped entry to
// file
// - `flush()` — ensures all buffered writes hit disk
// - `readBack(int n)` — reads and prints the last n lines from the log file
// - `rotateLogs(int max_files)` — if more than `max_files` log files exist in the directory, delete
// the oldest

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

class EventLogger {
public:
  EventLogger(const std::filesystem::path& file) : file_path{file} {
    std::filesystem::create_directories(file_path.parent_path());
  }

  void log(const std::string& source, const std::string& message) {
    time++;

    std::ofstream out(file_path, std::ios::app);
    out << "[" << time << "] " << source << ": " << message << "\n";
  }

  void flush() {
    std::ofstream out(file_path, std::ios::app);
    out.flush();
  }

  void readback(int n) {
    std::ifstream in(file_path);

    std::string line;
    std::getline(in, line);

    std::cout << line << "\n";
  }

private:
  std::filesystem::path file_path;
  int time{};
};