#include <iostream>

#include "memory"

// - Add a `CommandEvent` struct and overload `operator<<` for it so events print cleanly

struct CommandEvent {
  std::string cmd;
  float time;
  int priority;

  friend std::ostream& operator<<(std::ostream& os, const CommandEvent& cmd);
};

std::ostream& operator<<(std::ostream& os, const CommandEvent& cmd_evnt) {
  os << cmd_evnt.cmd << " " << cmd_evnt.time << " " << cmd_evnt.priority << "\n";

  return os;
}

int main() {
  CommandEvent test;
  test.cmd = "move";
  test.time = 1.2;
  test.priority = 1;

  std::cout << test;

  return 0;
}
