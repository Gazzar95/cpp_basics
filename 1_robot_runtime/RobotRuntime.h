#include <string>

#include "SensorBuffer.h"
#pragma once

class RobotRuntime {
public:
  //--------------------
  // Basic Constructor
  RobotRuntime(const std::string& name, int ID, SensorBuffer&& Buffer);
  RobotRuntime(RobotRuntime&& other) noexcept;

  void run();

  void printStatus();

  ~RobotRuntime();

private:
  std::string name;
  int session_id{};
  SensorBuffer buffer;
  // cmd stack params - FILO
  std::stack<std::string> data_mov;
  // log list params
  std::list<CommandEvent> log;

  int timestep{};
};
