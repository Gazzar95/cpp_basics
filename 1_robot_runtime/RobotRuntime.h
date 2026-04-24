

#ifndef ROBOT_RUNTIME_H
#define ROBOT_RUNTIME_H

#include <string>

#include "SensorBuffer.h"

class RobotRuntime {
public:
  RobotRuntime(const std::string& na, int ID, int capacity);

  void run();

  void printStatus();

  ~RobotRuntime();

private:
  std::string name;
  int session_id{};
  SensorBuffer buffer;
};

#endif