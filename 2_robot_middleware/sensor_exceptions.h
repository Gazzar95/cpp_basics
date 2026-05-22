// Exception hierarchy in `sensor_exceptions.h`:
// ```
// SensorException (inherits std::runtime_error)
// ├── SensorReadError     — sensor returned out-of-range data
// ├── SensorTimeoutError  — sensor didn't respond
// └── CalibrationError    — calibration failed
// ```
#pragma once
#include <stdexcept>
#include <string>

class SensorException : public std::runtime_error {
public:
  SensorException(const std::string& msg) : std::runtime_error(msg) {}
};

class SensorReadError : public SensorException {
public:
  SensorReadError(const std::string& msg) : SensorException(msg) {}
};

class SensorTimeoutError : public SensorException {
public:
  SensorTimeoutError(const std::string& msg) : SensorException(msg) {}
};

class CalibrationError : public SensorException {
public:
  CalibrationError(const std::string& msg) : SensorException(msg) {}
};
