#include <iostream>

class SensorBuffer {
public:
  SensorBuffer(int cap);

  void push(double value);
  void stats();
  void dump();
  bool isFull();
  bool isEmpty();

  ~SensorBuffer();

private:
  int n{};
  int k{};
  double* data;
  int capacity{};

  double min();
  double max();
  double avg();
};