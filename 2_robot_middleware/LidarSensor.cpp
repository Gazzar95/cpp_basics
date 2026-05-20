// - `LidarSensor` — `read()` returns a simulated distance (random float in range),
// `type()` returns "LIDAR"
#pragma once
#include <random>
#include <string>

#include "LidaSensor.h"
#include "Sensor.h"

LidarSensor::LidarSensor(const std::string& name, int id) : Sensor(name, id) {}

double LidarSensor::read() const { return (rand() % 1000) / 100.0; };

std::string LidarSensor::type() const { return "Lidar"; }
