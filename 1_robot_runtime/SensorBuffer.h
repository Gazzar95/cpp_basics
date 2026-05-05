#pragma once
#include <iostream>
#include <memory>

class SensorBuffer {
public:
  SensorBuffer(int cap);
  SensorBuffer(SensorBuffer&& Other) noexcept;

  SensorBuffer& operator=(SensorBuffer&& other) noexcept;
  bool operator==(const SensorBuffer& buffer_comp) const noexcept {}
  double SensorBuffer::operator[](int i) const {}
  std::ostream& SensorBuffer::operator<<(std::ostream& os, const SensorBuffer& buf) const {}
  friend std::ostream& operator<<(std::ostream& os, const SensorBuffer& buf);
  SensorBuffer::operator bool() const {}

  void push(double value);
  void stats();
  void dump();
  bool isFull();
  bool isEmpty();

private:
  int capacity{};
  int n{};
  int k{};
  std::unique_ptr<double[]> data;

  double min();
  double max();
  double avg();
};