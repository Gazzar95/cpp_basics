//- `ImuSensor` — `read()` returns a simulated angular velocity,
//`type()` returns "IMU", overrides `calibrate()` with IMU-specific behavior
#pragma once
#include "ImuSensor.h"

#include <string>

#include "Sensor.h"

ImuSensor::ImuSensor(const std::string& name, int id) : Sensor(name, id) {}

double ImuSensor::read() const { return (rand() % 1000) / 100.0; }
std::string ImuSensor::type() const { return "IMU"; }
void ImuSensor::calibrate() { std::cout << "IMU calibrated" << "\n"; }
