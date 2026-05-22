// - `LidarSensor` — `read()` returns a simulated distance (random float in range),
// `type()` returns "LIDAR"
#include "LidarSensor.h"

#include <random>
#include <string>

#include "Sensor.h"
#include "sensor_exceptions.h"

double LidarSensor::read() const {
  auto measure = (rand() % 1000) / 100.0;

  if (rand() % 100 < 20) {
    throw SensorReadError("Lidar bad value:" + std::to_string(measure));
  }
  if (rand() % 100 < 5) {
    throw SensorTimeoutError("Sensor timed out");
  }

  return measure;
};

std::string LidarSensor::type() const { return "Lidar"; }
