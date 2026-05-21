// Base class `Sensor` (abstract) in `sensor.h`:
// ```
// - string name_
// - int id_
// - pure virtual double read() const = 0
// - pure virtual string type() const = 0
// - virtual void calibrate() — default implementation prints "Default calibration"
// - virtual ~Sensor() — always virtual in a base class
// - operator<< overload that calls type() and read()
// ```
#pragma once
#include <iostream>
#include <string>

class Sensor {
public:
  Sensor(const std::string& name, int id) : name_{name}, id_{id} {}

  virtual double read() const = 0;
  virtual std::string type() const = 0;
  virtual void calibrate() { std::cout << "Default calibration" << "\n"; }
  int id() const { return id_; }

  friend std::ostream& operator<<(std::ostream& os, const Sensor& sensor);

  virtual ~Sensor() = default;

protected:
  std::string name_;
  int id_;
};