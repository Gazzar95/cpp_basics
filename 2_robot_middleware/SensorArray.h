#pragma once
#include <memory>
#include <vector>

#include "Sensor.h"

class SensorArray {
public:
  void addSensor(std::unique_ptr<Sensor> s);

  void readAll();
  void calibrateAll();

  Sensor* getSensorById(int id);

private:
  std::vector<std::unique_ptr<Sensor>> all_sensors;
};