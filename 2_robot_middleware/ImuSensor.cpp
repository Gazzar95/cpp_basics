//- `ImuSensor` — `read()` returns a simulated angular velocity,
//`type()` returns "IMU", overrides `calibrate()` with IMU-specific behavior
#include "ImuSensor.h"

#include <iostream>
#include <string>

#include "Sensor.h"
#include "sensor_exceptions.h"

double ImuSensor::read() const { return (rand() % 1000) / 100.0; }
std::string ImuSensor::type() const noexcept { return "IMU"; }
void ImuSensor::calibrate() { std::cout << "IMU calibrated\n"; }
void ImuSensor::calibrate(bool force_fail) {
  if (force_fail) {
    throw CalibrationError("calibration failed");
  }
  std::cout << "IMU calibrated" << "\n";
}
