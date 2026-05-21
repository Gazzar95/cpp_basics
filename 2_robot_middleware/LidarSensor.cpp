// - `LidarSensor` — `read()` returns a simulated distance (random float in range),
// `type()` returns "LIDAR"
#include "LidarSensor.h"

#include <random>
#include <string>

#include "Sensor.h"

double LidarSensor::read() const { return (rand() % 1000) / 100.0; };

std::string LidarSensor::type() const { return "Lidar"; }
