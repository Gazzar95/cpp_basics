
#include "Sensor.h"

#include <iostream>
#include <string>

std::ostream& operator<<(std::ostream& os, const Sensor& sensor) {
  os << sensor.read() << " " << sensor.type();
  return os;
}