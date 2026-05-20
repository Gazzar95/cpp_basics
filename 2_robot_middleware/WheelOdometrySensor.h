
// - `WheelOdometrySensor` — `read()` returns a simulated velocity,
//`type()` returns "ODOMETRY"
#pragma once
#include <string>

#include "Sensor.h"

class WheelOdometrySensor : public Sensor {
public:
  WheelOdometrySensor(const std::string& name, int id) : Sensor(name, id) {}

  double read() const override {}
  std::string type() const override {}
};