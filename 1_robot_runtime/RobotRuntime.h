#pragma once

#include <string>

#include "SensorBuffer.h"

class RobotRuntime {
public:
  RobotRuntime(const std::string& na, int ID, SensorBuffer&& Buffer);

  void run();

  void printStatus();

  ~RobotRuntime();

private:
  std::string name;
  int session_id{};
  SensorBuffer buffer;
};
