
// - `WheelOdometrySensor` — `read()` returns a simulated velocity,
//`type()` returns "ODOMETRY"
#include "WheelOdometrySensor.h"

#include <string>

#include "Sensor.h"

double WheelOdometrySensor::read() const { return (rand() % 1000) / 10.0; }
std::string WheelOdometrySensor::type() const { return "ODOMETRY"; }
