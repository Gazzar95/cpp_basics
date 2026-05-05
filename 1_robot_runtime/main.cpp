
#include "SensorBuffer.h"
#include "memory"

//- Add a second class `SharedLogger` that holds a `std::shared_ptr` to a log buffer
// — demonstrate two objects sharing ownership and the buffer surviving until both are destroyed

class SharedLogger {
public:
  std::shared_ptr<SensorBuffer> buffer = std::make_shared<SensorBuffer>(10);
};

int main() {
  std::shared_ptr<SensorBuffer> shared_buf = std::make_shared<SensorBuffer>(10);
  {
    SharedLogger Log1;
    Log1.buffer = shared_buf;

    {
      SharedLogger Log2;
      Log2.buffer = shared_buf;

      std::cout << "Log status:" << shared_buf.use_count() << "\n";
    }
    std::cout << "Log status:" << shared_buf.use_count() << "\n";
  }
  std::cout << "Log status:" << shared_buf.use_count() << "\n";

  return 0;
}