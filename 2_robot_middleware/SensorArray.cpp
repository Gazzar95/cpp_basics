// `SensorArray` class:
// - Holds `std::vector<std::unique_ptr<Sensor>>`
// - `addSensor(std::unique_ptr<Sensor> s)`
// - `readAll()` — iterates and prints each sensor's output via `operator<<`
// - `calibrateAll()` — calls calibrate() on each
// - `getSensorById(int id)` — returns raw pointer (non-owning), use `dynamic_cast` to let caller
// check the concrete type
#include <memory>

#include "Sensor.h"

void SensorArray::addSensor(std::unique_ptr<Sensor> s) { all_sensors.push_back(std::move(s)); }

void SensorArray::readAll() {
  for (auto& sensor : all_sensors) {
    std::cout << *sensor << "\n";
  }
}

void SensorArray::calibrateAll() {
  for (const auto& sensor : all_sensors) {
    sensor->calibrate();
  }
}

Sensor* SensorArray::getSensorById(int id) {
  for (const auto& sensor : all_sensors) {
    if (id == sensor->id()) {
      return sensor.get();
    }
  }
  return nullptr;
}
