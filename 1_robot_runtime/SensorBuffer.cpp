/*
Requirements:
- Class `SensorBuffer` in `sensor_buffer.h` / `sensor_buffer.cpp`
- Constructor takes `capacity` as argument, allocates buffer on the heap with `new`
- Destructor frees with `delete[]`
- Methods: `push(double value)`, `stats()` (prints min, max, mean), `isFull()`, `isEmpty()`
- In `main.cpp`, instantiate with `SensorBuffer buffer{10};` (uniform init)
- Handle the edge cases: push to full buffer overwrites oldest (ring behavior)
*/
#include "SensorBuffer.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
//--------------------
// Basic Constructor
SensorBuffer::SensorBuffer(int cap)
    : capacity{cap}, n{0}, k{0}, data{std::make_unique<double[]>(capacity)} {};

// Move Constructor
SensorBuffer::SensorBuffer(SensorBuffer&& other) noexcept
    : capacity(other.capacity), n{other.n}, k{other.k}, data{std::move(other.data)} {
  other.capacity = 0;
  other.n = 0;
  other.k = 0;
  other.data = nullptr;
};
//--------------------
// xx- Overload `operator<<` for `SensorBuffer` so `std::cout << buffer` prints its contents
// xx- Overload `operator[]` on `SensorBuffer` for indexed read access
// xx- Overload `operator==` on `SensorBuffer` to compare two buffers by their contents
// - Add an explicit conversion operator `explicit operator bool()` that returns `true` if the
// buffer has data
// - Add a `CommandEvent` struct and overload `operator<<` for it so events print cleanly

// operator=() Overloading
SensorBuffer& SensorBuffer::operator=(SensorBuffer&& other) noexcept {
  if (this != &other) {
    capacity = other.capacity;
    n = other.n;
    k = other.k;
    data = std::move(other.data);

    other.capacity = 0;
    other.n = 0;
    other.k = 0;
  };
  return *this;
};

bool SensorBuffer::operator==(const SensorBuffer& other) const noexcept {
  if (n != other.n) return false;

  for (int i = 0; i < n; i++) {
    if (data[i] != other.data[i]) return false;
  }
  return true;
}

double SensorBuffer::operator[](int i) const {
  if (i < 0 || i < n) {
    throw std::out_of_range("Index out of bounds");
  }
  return data[i];
}

std::ostream& operator<<(std::ostream& os, const SensorBuffer& buf) {
  for (int i = 0; i < buf.n; i++) {
    os << buf.data[i] << "\n";
  }
  return os;
}

SensorBuffer::operator bool() const { return n > 0; };

//--------------------
// Methods
void SensorBuffer::push(double value) {
  if (isFull()) {
    data[k] = value;
    k = (k + 1) % capacity;
  } else {
    data[n] = value;
    n++;
  }
};

void SensorBuffer::stats() {
  if (isEmpty()) {
    std::cout << "Error: no data available";
  } else {
    double min_d{};
    double max_d{};
    double avg_d{};
    min_d = min();
    max_d = max();
    avg_d = avg();
    std::cout << "COUNT=" << n << " MIN=" << min_d << " MAX=" << max_d << " AVG=" << avg_d << "\n";
  }
}

void SensorBuffer::dump() {
  std::cout << "SAMPLES: ";

  for (int i{0}; i < n; i++) {
    std::cout << data[i] << " ";
  }
  std::cout << "\n";
}

bool SensorBuffer::isFull() { return n == capacity; }
bool SensorBuffer::isEmpty() { return n == 0; }

double SensorBuffer::min() {
  double min{data[0]};
  for (int i{}; i < n; i++) {
    if (data[i] < min) {
      min = data[i];
    }
  }
  return min;
};

double SensorBuffer::max() {
  double max{data[0]};
  for (int i{}; i < n; i++) {
    if (data[i] > max) {
      max = data[i];
    }
  }
  return max;
}

double SensorBuffer::avg() {
  double sum{0};
  double avg;
  for (int i{0}; i < n; i++) {
    sum = sum + data[i];
  }

  avg = (sum) / n;

  return avg;
}
//--------------------