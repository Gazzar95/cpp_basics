//- `ImuSensor` — `read()` returns a simulated angular velocity,
//`type()` returns "IMU", overrides `calibrate()` with IMU-specific behavior
#pragma once
#include <string>

#include "Sensor.h"

class ImuSensor : public Sensor {
public:
  ImuSensor(const std::string& name, int id) : Sensor(name, id) {}

  double read() const override;
  std::string type() const noexcept override;
  void calibrate() override;
  void calibrate(bool force_fail);
};
