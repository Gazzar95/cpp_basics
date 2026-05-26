// - `LidarSensor` — `read()` returns a simulated distance (random float in range),
// `type()` returns "LIDAR"
#pragma once
#include <random>
#include <string>

#include "Sensor.h"

class LidarSensor : public Sensor {
public:
  LidarSensor(const std::string& name, int id) : Sensor(name, id) {}

  double read() const override;

  std::string type() const noexcept override;
};
