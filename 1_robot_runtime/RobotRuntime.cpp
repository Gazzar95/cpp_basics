
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
#include <string>

#include "RobotError.h"
#include "SensorBuffer.h"
#include "cmdstack.h"
#include "telemetry.h"

// constructor
RobotRuntime::RobotRuntime(const std::string& na, int ID, SensorBuffer&& buffer)
    : name{na}, session_id{ID}, buffer{std::move(buffer)} {};

// main loop--------------------
void RobotRuntime::run() {
  // While loop bool
  bool run = true;

  // Tel params
  double value;

  // cmd stack params
  std::string cmd;
  std::string mov;
  std::string data_mov[30];
  int k{0};

  while (run)

  {
    try {
      std::cin >> cmd;

      if (cmd == "sample") {
        std::cin >> value;
        if (std::cin.fail()) {
          std::cout << "ERROR: value must be a number\n";
          std::cin.clear();             // clear the error flag
          std::cin.ignore(1000, '\n');  // discard the bad input
        } else {
          buffer.push(value);
        }
      } else if (cmd == "stats") {
        buffer.stats();
      } else if (cmd == "dump") {
        buffer.dump();
      }

      else if (cmd == "push") {
        std::cin >> mov;
        push(data_mov, mov, k);
      } else if (cmd == "pop") {
        pop(data_mov, k);
      } else if (cmd == "peek") {
        peek(data_mov, k);
      } else if (cmd == "quit") {
        run = false;
      } else {
        throw InvalidCommandError("ERROR:incorrect command input");
      }
    }

    catch (const RobotError& e) {
      std::cout << e.what() << "\n";
    }
  }
}

void RobotRuntime::printStatus() {
  // prints name, session ID, buffer stats
  std::cout << "Name: " << name << "\n";
  std::cout << "Session ID: " << session_id << "\n";
  buffer.stats();
}

RobotRuntime::~RobotRuntime() { std::cout << "Runtime shutting down: " << name << "\n"; };
