// - Instantiate with `std::make_unique<RobotRuntime>("Atlas", 1)`
// - Call `->run()`
// - Outer try/catch for fatal errors

#include <iostream>
#include <memory>

#include "RobotError.h"
#include "RobotRuntime.h"
#include "SensorBuffer.h"

int main() {
  try {
    auto runtime = std::make_unique<RobotRuntime>("run1", 1, SensorBuffer(10));

    runtime->run();
  } catch (const std::exception& e) {
    std::cout << e.what() << "\n";
  }

  return 0;
}
