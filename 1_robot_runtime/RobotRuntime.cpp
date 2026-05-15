
/*
**Requirements:**
- `RobotRuntime` class in `robot_runtime.h` / `robot_runtime.cpp`
- Private members: `SensorBuffer`, a name string, a session ID (just an int)
- Constructor: `RobotRuntime(const std::string& name, int session_id)`
- Public method: `run()` — contains the command loop from Assignment 2
- Public method: `printStatus()` — prints name, session ID, buffer stats
- Explicit destructor that prints "Runtime shutting down: [name]" on exit
- No public data members — everything goes through methods
*/

#include "RobotRuntime.h"

#include <iostream>
#include <list>
#include <stack>
#include <string>

#include "CommandEvent.h"
#include "RobotError.h"
#include "SensorBuffer.h"

//--------------------
// Basic Constructor
RobotRuntime::RobotRuntime(const std::string& na, int ID, SensorBuffer&& buffer)
    : name{na}, session_id{ID}, buffer{std::move(buffer)} {}
RobotRuntime::RobotRuntime(RobotRuntime&& other) noexcept
    : name(std::move(other.name)), session_id{other.session_id}, buffer{std::move(other.buffer)} {
  other.session_id = 0;
};

//--------------------
// main loop
void RobotRuntime::run() {
  bool run = true;

  // cmd stack params - FILO
  std::string mov;
  std::string cmd;
  // Tel params
  double value;

  while (run) {
    try {
      std::cin >> cmd;
      timestep++;

      if (cmd == "sample") {
        std::cin >> value;

        if (std::cin.fail()) {
          std::cout << "ERROR: value must be a number\n";
          std::cin.clear();             // clear the error flag
          std::cin.ignore(1000, '\n');  // discard the bad input
        } else {
          buffer.push(value);
          log.push_back({timestep, cmd + " " + std::to_string(value)});
        }
      } else if (cmd == "stats") {
        buffer.stats();
        log.push_back({timestep, cmd});

      } else if (cmd == "dump") {
        buffer.dump();
        log.push_back({timestep, cmd});
      }

      else if (cmd == "push") {
        std::cin >> mov;
        log.push_back({timestep, cmd + " " + mov});

        data_mov.push(mov);
      } else if (cmd == "pop") {
        log.push_back({timestep, cmd});

        if (data_mov.empty()) {
          throw CommandOverflowError("ERROR: command list is empty");
        }
        data_mov.pop();
      } else if (cmd == "peek") {
        log.push_back({timestep, cmd});
        if (data_mov.empty()) {
          throw CommandOverflowError("ERROR: command list is empty");
        }
        std::cout << data_mov.top() << "\n";
      } else if (cmd == "quit") {
        log.push_back({timestep, cmd});
        run = false;
      } else if (cmd == "log") {
        log.push_back({timestep, cmd});
        for (const auto& event : log) {
          std::cout << event << "\n";
        }
      } else {
        throw InvalidCommandError("ERROR:incorrect command input");
      }
    }

    catch (const RobotError& e) {
      std::cout << e.what() << "\n";
    }
  }
}

//--------------------
// Method for status
void RobotRuntime::printStatus() {
  // prints name, session ID, buffer stats
  std::cout << "Name: " << name << "\n";
  std::cout << "Session ID: " << session_id << "\n";
  buffer.stats();
  std::cout << "Data QTY: " << data_mov.size() << "\n";
  std::cout << "Log Size: " << log.size() << "\n";
}

//--------------------
// Basic De-Constructor
RobotRuntime::~RobotRuntime() { std::cout << "Runtime shutting down: " << name << "\n"; };
