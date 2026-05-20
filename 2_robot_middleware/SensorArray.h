#pragma once
#include <memory>

#include "Sensor.h"

class SensorArray {
public:
  void addSensor(std::unique_ptr<Sensor> s) { all_sensors.push_back(std::move(s)); }

  void readAll() {
    for (auto& sensor : all_sensors) {
      std::cout << *sensor << "\n";
    }
  }

  void calibrateAll() {
    for (const auto& sensor : all_sensors) {
      sensor->calibrate();
    }
  }

  Sensor* getSensorById(int id) {
    for (const auto& sensor : all_sensors) {
      if (id == sensor->id()) {
        return sensor.get();
      }
    }
    return nullptr;
  }

private:
  std::vector<std::unique_ptr<Sensor>> all_sensors;
};