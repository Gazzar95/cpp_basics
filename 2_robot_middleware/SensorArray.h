// In `SensorArray`, add:
// - `filterSensors(std::function<bool(const Sensor&)> predicate)`
//      — returns vector of non-owning pointers to sensors matching the predicate
// - `transformReadings(std::function<double(double)> transform)`
//      — applies transform to all current readings, returns vector of results
// - `sortSensorsBy(std::function<bool(const Sensor&, const Sensor&)> comparator)`
//      — sorts internal vector in-place

#pragma once
#include <functional>
#include <memory>
#include <vector>

#include "Sensor.h"

class SensorArray {
public:
  void addSensor(std::unique_ptr<Sensor> s);

  void readAll();
  void calibrateAll();
  Sensor* getSensorById(int id) noexcept;
  std::vector<Sensor*> filterSensors(std::function<bool(const Sensor&)> filter);

private:
  std::vector<std::unique_ptr<Sensor>> all_sensors;
};
