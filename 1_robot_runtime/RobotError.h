/*
**Requirements:**
- Create a custom exception class `RobotError` that inherits from `std::runtime_error`
- Subclass it: `SensorOverflowError`, `InvalidCommandError`
- Throw `SensorOverflowError` if a sensor value is out of range (e.g. > 1000.0)
- Throw `InvalidCommandError` if an unrecognized command is entered
- Wrap the command loop in `RobotRuntime::run()` with try/catch — catch specific errors, log them,
and continue running (don't crash on bad input)
- Add a catch-all at the outermost level in `main.cpp`
*/

#include <iostream>
#include <stdexcept>
#include <string>

class RobotError : public std::runtime_error {
public:
  RobotError(const std::string& msg) : std::runtime_error(msg) {}
};

class SensorOverflowError : public RobotError {
public:
  SensorOverflowError(const std::string& msg) : RobotError(msg) {}
};

class CommandOverflowError : public RobotError {
public:
  CommandOverflowError(const std::string& msg) : RobotError(msg) {}
};

class InvalidCommandError : public RobotError {
public:
  InvalidCommandError(const std::string& msg) : RobotError(msg) {}
};