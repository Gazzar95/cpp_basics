# Packt Modern C++ — Course 3 Assignments
## Modern C++ Features & Concurrency (Robotics-Focused)

**Theme:** You're adding real-time capabilities to your `RobotMiddleware` — concurrent sensor reading, thread-safe logging, and asynchronous task execution. By the end of Course 3, your middleware will run multiple subsystems in parallel, the same way a real ROS 2 node does. This is the final piece before you start writing actual ROS 2 code.

**Ground rules:**
- Continue building on `RobotMiddleware` from Course 2
- Compile with `g++ -std=c++17 -Wall -Wextra -pthread`
- All shared data must be protected — no data races
- Each assignment introduces one concurrency or C++17 concept; the capstone wires them together

---

## Module 1 — `std::thread` & Basic Concurrency
**Topics:** creating threads, joining/detaching, thread arguments, thread-local storage

### Assignment 1: Concurrent Sensor Reads

Make your `SensorArray` read sensors in parallel instead of sequentially.

**Requirements:**

`ConcurrentSensorArray` class (extends or replaces `SensorArray`):
- `readAllParallel()` — spawns one `std::thread` per sensor, each calls `sensor->read()` and stores the result
- Use `std::vector<std::thread>` to manage the threads
- Join all threads before returning results
- Handle the case where a sensor throws during `read()` — the thread must catch internally, not crash the program
- Store results in a `std::vector<std::optional<double>>` — `std::nullopt` for failed reads

**Thread-local storage demo:**
- Add a `thread_local int read_count` that each thread increments — demonstrate that each thread has its own counter

**Demonstrate:**
```cpp
ConcurrentSensorArray sensors;
sensors.addSensor(std::make_unique<LidarSensor>("front", 1));
sensors.addSensor(std::make_unique<ImuSensor>("imu", 2));
sensors.addSensor(std::make_unique<WheelOdometrySensor>("odom", 3));

auto results = sensors.readAllParallel();
// results: [2.47, 0.31, 1.05] or [2.47, nullopt, 1.05] if IMU failed
```

**What you're internalizing:** Thread creation and lifecycle, why you must always join or detach, how to handle exceptions in threads. In ROS 2, the executor runs your callbacks on a thread pool — understanding threads is understanding how your code actually executes.

---

## Module 2 — Mutexes & Thread Safety
**Topics:** `std::mutex`, `std::lock_guard`, `std::unique_lock`, deadlock avoidance, `std::scoped_lock`

### Assignment 2: Thread-Safe Logger

Make your `EventLogger` from Course 2 safe for concurrent writes from multiple threads.

**Requirements:**

`ThreadSafeLogger` class (wraps or replaces `EventLogger`):
- Internal `std::mutex` protects all file writes
- `log(const std::string& source, const std::string& message)` — acquires lock with `std::lock_guard` before writing
- `logBatch(const std::vector<std::string>& messages)` — acquires lock once for the entire batch (not per-message)
- `getStats()` — returns a struct with `{total_logs, errors_logged, last_timestamp}` — protected by a separate `std::mutex` (practice with multiple locks)
- Use `std::scoped_lock` when you need to lock both mutexes simultaneously (e.g. in a `flush()` that updates stats and writes to file)

**Deadlock demonstration:**
- Write a commented-out code block that would deadlock (lock A then B in one thread, lock B then A in another)
- Below it, show the fix using `std::scoped_lock`

**Demonstrate:**
```cpp
ThreadSafeLogger logger{"logs/runtime.log"};

std::thread t1([&]() {
    for (int i = 0; i < 100; ++i)
        logger.log("sensor", "Reading " + std::to_string(i));
});

std::thread t2([&]() {
    for (int i = 0; i < 100; ++i)
        logger.log("planner", "Planning cycle " + std::to_string(i));
});

t1.join();
t2.join();
// Log file should have 200 entries, no interleaving or corruption
```

**What you're internalizing:** Why shared mutable state is dangerous, the lock guard pattern (RAII for mutexes — same principle as smart pointers for memory), deadlock causes and prevention. ROS 2 callback groups and multi-threaded executors require exactly this understanding.

---

## Module 3 — `std::async`, Futures & Promises
**Topics:** `std::async`, `std::future`, `std::promise`, launch policies, task-based concurrency

### Assignment 3: Async Task Execution

Upgrade your `TaskScheduler` to execute tasks asynchronously and return results through futures.

**Requirements:**

Add to `TaskScheduler`:
- `executeAsync(int task_id)` — returns `std::future<TaskResult>` where `TaskResult` is a struct with `{task_id, success, duration_ms, message}`
- Internally uses `std::async(std::launch::async, ...)` to run the task on a separate thread
- The task function should simulate work with `std::this_thread::sleep_for`
- `executeNextReady()` — finds the highest-priority ready task, launches it async, returns the future

`TaskResult` struct:
```
int task_id
bool success
double duration_ms
std::string message
```

**Promise/future demo — manual handoff:**
```cpp
// Simulate a sensor calibration where one thread does the work
// and another thread waits for the result
std::promise<bool> cal_promise;
std::future<bool> cal_future = cal_promise.get_future();

std::thread worker([&]() {
    // simulate calibration
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    cal_promise.set_value(true);  // signal success
});

// Main thread blocks here until result is ready
bool calibrated = cal_future.get();
worker.join();
```

**Demonstrate:**
```cpp
TaskScheduler scheduler;
scheduler.addTask({1, "check_battery", 3, 0.5});
scheduler.addTask({2, "navigate_to_goal", 5, 2.0});

auto future1 = scheduler.executeAsync(1);
auto future2 = scheduler.executeAsync(2);

// Both tasks running concurrently
auto result1 = future1.get();  // blocks until done
auto result2 = future2.get();

std::cout << result1 << "\n";  // uses operator<< on TaskResult
std::cout << result2 << "\n";
```

**What you're internalizing:** Task-based concurrency vs thread-based (async is higher-level and usually preferred), futures as a mechanism to get results from async work, promises for manual cross-thread communication. ROS 2 action servers use a very similar future/result pattern.

---

## Module 4 — Condition Variables & Producer-Consumer
**Topics:** `std::condition_variable`, producer-consumer pattern, `std::atomic`

### Assignment 4: Sensor Data Queue

Build a thread-safe queue that decouples sensor reading (producer) from data processing (consumer) — this is the fundamental pattern in any real-time robotics system.

**Requirements:**

`ThreadSafeQueue<T>` class template:
```cpp
template<typename T>
class ThreadSafeQueue {
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> shutdown_{false};

public:
    void push(T value);                          // notifies one waiting consumer
    std::optional<T> pop(std::chrono::milliseconds timeout);  // blocks until data or timeout
    void requestShutdown();                      // sets flag, notifies all waiters
    bool isShutdown() const;
    size_t size() const;
};
```

**Producer-consumer demo:**
```cpp
ThreadSafeQueue<double> sensor_queue;

// Producer thread — reads sensors and pushes to queue
std::thread producer([&]() {
    for (int i = 0; i < 50; ++i) {
        double reading = /* simulate sensor read */;
        sensor_queue.push(reading);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    sensor_queue.requestShutdown();
});

// Consumer thread — processes readings from queue
std::thread consumer([&]() {
    while (!sensor_queue.isShutdown()) {
        auto val = sensor_queue.pop(std::chrono::milliseconds(100));
        if (val) {
            // process the reading through Pipeline<double>
        }
    }
});

producer.join();
consumer.join();
```

**`std::atomic` usage:**
- The `shutdown_` flag is `std::atomic<bool>` — explain in a comment why a regular `bool` would be a data race
- Add an `std::atomic<int> processed_count` to the consumer that tracks how many readings have been processed — no mutex needed

**What you're internalizing:** The producer-consumer pattern is *the* core pattern in robotics middleware. ROS 2's intra-process communication, message queues, and callback buffers all work this way. Condition variables are how threads wait efficiently without spinning. Atomics are for simple shared counters/flags where a mutex would be overkill.

---

## Module 5 — Essential C++17 Features
**Topics:** structured bindings, `std::optional`, `std::variant`, `std::string_view`, `if constexpr`, fold expressions

*You've already used `std::optional` and `if constexpr` in Course 2. This module rounds out the C++17 features you'll encounter in modern ROS 2 code.*

### Assignment 5: Modernize the Middleware

Refactor your existing codebase to use C++17 features where they improve clarity.

**Requirements:**

**Structured bindings** — use wherever you unpack pairs, tuples, or structs:
```cpp
// Before:
auto result = scheduler.getStats();
int total = result.total;
int pending = result.pending;

// After:
auto [total, pending] = scheduler.getStats();
```
Refactor at least 3 places in your codebase to use structured bindings.

**`std::variant` for sensor readings** — replace raw doubles with typed readings:
```cpp
using SensorReading = std::variant<double, std::string, bool>;
// double for numeric readings, string for status messages, bool for binary sensors

// Use std::visit to process:
std::visit([](auto&& val) {
    using T = std::decay_t<decltype(val)>;
    if constexpr (std::is_same_v<T, double>) {
        std::cout << "Numeric: " << val << "\n";
    } else if constexpr (std::is_same_v<T, std::string>) {
        std::cout << "Status: " << val << "\n";
    } else if constexpr (std::is_same_v<T, bool>) {
        std::cout << "Binary: " << (val ? "ON" : "OFF") << "\n";
    }
}, reading);
```

**`std::string_view`** — use for function parameters that only read strings:
```cpp
// Before:
void log(const std::string& source, const std::string& message);

// After:
void log(std::string_view source, std::string_view message);
```
Refactor your logger and command parser to use `string_view` where appropriate. Add a comment explaining when `string_view` is unsafe (dangling reference to a temporary).

**Fold expressions** — add a variadic `logMultiple` to your logger:
```cpp
template<typename... Args>
void logMultiple(Args&&... args) {
    (log(std::forward<Args>(args)), ...);  // fold expression — calls log on each arg
}
```

**What you're internalizing:** These features make C++ code more readable and less error-prone. `std::variant` is a type-safe union (used in ROS 2 parameter handling). `string_view` avoids unnecessary copies. Structured bindings clean up tuple/struct unpacking. You'll read all of these in modern ROS 2 source.

---

## Course 3 Capstone: Concurrent RobotMiddleware

Upgrade your `RobotMiddleware` from Course 2 to run its subsystems concurrently — the final step before you start writing real ROS 2 nodes.

**Architecture:**
```
ConcurrentMiddleware
├── ConcurrentSensorArray    (reads sensors in parallel)
├── ThreadSafeQueue<double>  (decouples reading from processing)
├── Pipeline<double>         (processes readings — consumer side)
├── TaskScheduler            (async task execution)
├── ThreadSafeLogger         (concurrent-safe file logging)
└── RobotRuntime*            (non-owning pointer to Course 1 runtime)
```

**Full requirements:**

`ConcurrentMiddleware` class:
- Constructor takes a `std::filesystem::path` for logs, a `RobotRuntime*`, and an `int num_cycles`
- `start()` launches three threads:
  1. **Sensor thread** — runs in a loop, reads all sensors in parallel, pushes valid readings to `ThreadSafeQueue`, logs failures
  2. **Processing thread** — consumes from the queue, runs readings through `Pipeline<double>`, computes running statistics using `std::atomic<double>` for the mean
  3. **Task thread** — checks for ready tasks each cycle, executes them via `std::async`, logs results
- `stop()` — sets a `std::atomic<bool> running_` flag to false, calls `requestShutdown()` on the queue, joins all threads
- `getStatus()` — returns a struct with readings processed, tasks completed, errors logged (all atomics or mutex-protected)

**Graceful shutdown:**
- All threads check `running_` each loop iteration
- Queue consumers handle the shutdown signal (return `nullopt`)
- Destructors log final stats before cleanup
- Smart pointers ensure no leaks even if a thread throws

**Use C++17 features throughout:**
- Structured bindings for unpacking results
- `std::optional` for fallible operations
- `std::variant<double, std::string>` for mixed sensor data
- `string_view` for log function parameters

**main.cpp:**
```cpp
auto runtime = std::make_unique<RobotRuntime>("Atlas", 1);
ConcurrentMiddleware middleware{
    std::filesystem::path{"logs/"},
    runtime.get(),
    100  // run 100 cycles
};

try {
    middleware.start();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    middleware.stop();

    auto [readings, tasks, errors] = middleware.getStatus();
    std::cout << "Readings: " << readings
              << " Tasks: " << tasks
              << " Errors: " << errors << "\n";
} catch (const std::exception& e) {
    std::cerr << "Fatal: " << e.what() << "\n";
}
```

**What you're proving:** You can build a multi-threaded C++ system with safe shared state, producer-consumer pipelines, async task execution, and graceful shutdown. This is directly analogous to how a ROS 2 node works — the executor manages threads, callbacks read sensors, publishers push to queues, and everything shuts down cleanly on `Ctrl+C`. After this capstone, writing a ROS 2 lifecycle node will feel familiar, not foreign.

---

## Suggested Pacing

| Assignment | Estimated Time |
|---|---|
| A1: Concurrent Sensor Reads | 3–4 hrs |
| A2: Thread-Safe Logger | 3–4 hrs |
| A3: Async Task Execution | 3–4 hrs |
| A4: Sensor Data Queue | 4–5 hrs |
| A5: Modernize with C++17 | 2–3 hrs |
| Capstone: Concurrent Middleware | 6–8 hrs |
| **Total** | **~21–28 hrs** |

Assignment 4 (producer-consumer with condition variables) is the most important one in this course. This is the pattern you'll use constantly in robotics — don't rush it.

---

## What You Skip from the Full Coursera Course 3

The following topics from the Packt Course 3 are intentionally omitted because they're either covered implicitly above or not critical for your near-term robotics goals:

- **CTAD (Class Template Argument Deduction):** Nice syntax sugar, learn it when you see it in code. Not worth a dedicated assignment.
- **Parallel algorithms (`std::execution::par`):** Conceptually simple once you understand threads. Read about it in 10 minutes when you need it.
- **`std::any`:** Rarely used in robotics code. `std::variant` covers the same need more safely.
- **Detailed `noexcept` semantics:** Covered sufficiently in Course 1 (move constructors) and Course 2 (exception handling).
- **Fold expressions in depth:** One usage in Assignment 5 is enough. Deep template metaprogramming is a post-hire skill.

If you encounter any of these in ROS 2 source later, you'll have enough context to look them up and understand them quickly.