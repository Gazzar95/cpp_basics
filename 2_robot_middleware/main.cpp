#include <iostream>
#include <memory>

#include "ImuSensor.h"
#include "LidarSensor.h"
#include "Sensor.h"
#include "SensorArray.h"

int main() {
  SensorArray array;
  array.addSensor(std::make_unique<LidarSensor>("front_lidar", 1));
  array.addSensor(std::make_unique<ImuSensor>("imu_0", 2));
  array.readAll();

  // dynamic_cast demonstration
  Sensor* s = array.getSensorById(2);
  if (auto* imu = dynamic_cast<ImuSensor*>(s)) {
    imu->calibrate();  //
  }

  return 0;
}