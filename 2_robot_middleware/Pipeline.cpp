// **`Pipeline<T>` class template:**
// ```cpp
// template<typename T>
// class Pipeline {
//     // Holds a DataBuffer<T, 64>
//     // addStage(std::function<T(T)> transform) — adds a processing step
//     // process(T input) -> T  — runs input through all stages in order
// };
// ```
#pragma once
#include <functional>
#include <vector>

#include "DataBuffer.h"

template <typename T>
class Pipeline {
public:
  template <typename F>
  void addStage(F&& function) {
    functions.push_back(std::forward<F>(function));
  }

  T process(T input) {
    T solution = input;

    for (const auto& function : functions) {
      solution = function(solution);
    }

    buffer.push(solution);

    return solution;
  }

private:
  std::vector<std::function<T(T)>> functions;
  DataBuffer<T, 64> buffer;
};