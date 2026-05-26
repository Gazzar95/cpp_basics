//`SensorArray` class:
// - Holds `std::vector<std::unique_ptr<Sensor>>`
// - `addSensor(std::unique_ptr<Sensor> s)`
// - `readAll()` — iterates and prints each sensor's output via `operator<<`
// - `calibrateAll()` — calls calibrate() on each
// - `getSensorById(int id)` — returns raw pointer (non-owning), use `dynamic_cast` to let caller
// check the concrete type
#include "SensorArray.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "Sensor.h"
#include "sensor_exceptions.h"

void SensorArray::addSensor(std::unique_ptr<Sensor> s) { all_sensors.push_back(std::move(s)); }

// - Use `std::nested_exception` / `std::throw_with_nested` to wrap a `SensorReadError`
// inside a higher-level `SensorArrayError` when more than 2 sensors fail in one pass
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

// In `SensorArray::readAll()`:
// - Catch per-sensor exceptions, log them, and continue reading
// remaining sensors — one bad sensor shouldn't stop the loop
// - Use `std::nested_exception` / `std::throw_with_nested` to wrap
// a `SensorReadError` inside a higher-level `SensorArrayError` when
// more than 2 sensors fail in one pass
