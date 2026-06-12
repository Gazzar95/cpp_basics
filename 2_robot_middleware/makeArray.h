// **Variadic template utility `makeArray`:**
// ```cpp
// template<typename T, typename... Args>
// std::array<T, sizeof...(Args)> makeArray(Args&&... args);
// // Usage: auto readings = makeArray<double>(1.0, 2.5, 3.7);
// ```
#include <array>
#include <utility>

template <typename T, typename... Args>
std::array<T, sizeof...(Args)> makeArray(Args&&... arg) {
  return {static_cast<T>(std::forward<Args>(arg))...};
}
