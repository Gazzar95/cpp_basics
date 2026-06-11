
/*
**`DataBuffer<T, size_t N>` class template:**
```cpp
template<typename T, std::size_t N>
class DataBuffer {
    // Fixed-capacity ring buffer, N is a non-type template parameter
    // push(T&& val)          — perfect forwarding
    // pop() -> std::optional<T>
    // peek() const -> const T&
    // size_t count() const
    // bool empty() const
    // iterator support (range-based for loop)
};
*/
#pragma once
#include <optional>

template <typename T, std::size_t N>
class DataBuffer {
public:
  void push(T value) {
    if (full()) {
      data[k] = value;
      k = (k + 1) % N;
    } else {
      data[n] = value;
      n++;
    }
  }
  std::optional<T> pop() {
    if (empty()) return std::nullopt;
    T value = data[k];
    k = (k + 1) % N;
    n--;
    return value;
  }
  const T& peek() const { return data[k]; }
  size_t count() const { return n; }
  bool full() const { return n == N; }
  bool empty() const { return n == 0; }

private:
  std::size_t n{};
  std::size_t k{};
  T data[N];
};