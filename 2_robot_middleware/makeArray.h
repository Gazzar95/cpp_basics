// **Variadic template utility `makeArray`:**
// ```cpp
// template<typename T, typename... Args>
// std::array<T, sizeof...(Args)> makeArray(Args&&... args);
// // Usage: auto readings = makeArray<double>(1.0, 2.5, 3.7);
// ```
#include <array>
#include <iomanip>
#include <utility>

template <typename T, typename... Args>
std::array<T, sizeof...(Args)> makeArray(Args&&... arg) {
  return {static_cast<T>(std::forward<Args>(arg))...};
}

template <typename T>
void printReading(const T& val) {
  if constexpr (std::is_floating_point_v<T>) {
    std::cout << std::fixed << std::setprecision(4) << val;
  } else if constexpr (std::is_integral_v<T>) {
    std::cout << "int: " << val;
  } else {
    std::cout << val;
  }
}