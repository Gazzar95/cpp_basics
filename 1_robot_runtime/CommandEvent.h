
#include <iostream>
#include <string>

struct CommandEvent {
  int timestamp;
  std::string cmd_log;

  friend std::ostream& operator<<(std::ostream& os, const CommandEvent& event);
};

std::ostream& operator<<(std::ostream& os, const CommandEvent& event) {
  os << "[" << event.timestamp << "] " << event.cmd_log;
  return os;
}
