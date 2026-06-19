//`SensorArray` class:
// - Holds `std::vector<std::unique_ptr<Sensor>>`
// - `addSensor(std::unique_ptr<Sensor> s)`
// - `readAll()` — iterates and prints each sensor's output via `operator<<`
// - `calibrateAll()` — calls calibrate() on each
// - `getSensorById(int id)` — returns raw pointer (non-owning), use `dynamic_cast` to let caller
// check the concrete type

// In `SensorArray`, add:

// - `transformReadings(std::function<double(double)> transform)`
// — applies transform to all current readings, returns vector of results
// - `sortSensorsBy(std::function<bool(const Sensor&, const Sensor&)> comparator)`
// — sorts internal vector in-place

#include "SensorArray.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "Sensor.h"
#include "sensor_exceptions.h"

void SensorArray::addSensor(std::unique_ptr<Sensor> s) { all_sensors.push_back(std::move(s)); }

void SensorArray::readAll() {
  int err_count{};

  for (auto& sensor : all_sensors) {
    try {
      std::cout << *sensor << "\n";
    } catch (const SensorException& err) {
      std::cout << err.what() << "\n";
      err_count++;

      if (err_count > 2) {
        std::throw_with_nested(SensorArrayError("more than two sensors failed"));
      }
    }
  }
}

void SensorArray::calibrateAll() {
  for (auto& sensor : all_sensors) {
    sensor->calibrate();
  }
}

Sensor* SensorArray::getSensorById(int id) noexcept {
  for (const auto& sensor : all_sensors) {
    if (id == sensor->id()) {
      return sensor.get();
    }
  }
  return nullptr;
}

std::vector<Sensor*> SensorArray::filterSensors(std::function<bool(const Sensor&)> filter) {
  std::vector<Sensor*> results;

  for (const auto& sensor : all_sensors) {
    if (filter(*sensor)) {
      results.push_back(sensor.get());
    }
  }

  return results;
}

std::vector<double> SensorArray::transformReading(std::function<double(double)> transform) {
  std::vector<double> transformed_readings;

  for (const auto& sensor : all_sensors) {
    transformed_readings.push_back(transform(sensor->read()));
  }

  return transformed_readings;
}

void SensorArray::sortSensorsBy(std::function<bool(const Sensor&, const Sensor&)> comparator) {
  std::sort(
      all_sensors.begin(), all_sensors.end(),
      [&comparator](const std::unique_ptr<Sensor>& a, const std::unique_ptr<Sensor>& b) {
        return comparator(*a, *b);
      });