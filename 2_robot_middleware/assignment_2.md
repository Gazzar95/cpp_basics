# Packt Modern C++ — Course 2 Assignments
## Advanced OOP & Generic Programming

**Theme:** You're building a robotics middleware layer — a simplified version of what sits between hardware drivers and a planner. By the end of Course 2 you'll have a `RobotMiddleware` system with a typed sensor hierarchy, a generic data pipeline, a file-backed event logger, and an STL-powered task queue. This is the kind of architecture you'll actually read in ROS 2 and Nav2 source.

**Ground rules:**
- Continue building on the `RobotRuntime` from Course 1 — these are new subsystems that plug into it
- Compile with `g++ -std=c++17 -Wall -Wextra`
- No raw `new`/`delete` — use smart pointers throughout
- Each assignment introduces one architectural component; the capstone wires them together

---

## Module 1 — Object-Oriented Programming
**Topics:** inheritance, composition, virtual functions, override/final, object slicing, dynamic_cast, abstract classes, multiple inheritance

This is the most important module in Course 2. Virtual dispatch, abstract interfaces, and dynamic_cast are everywhere in robotics software — ROS 2 plugins, Nav2 behaviors, controller interfaces all use this pattern.

### Assignment 1: Sensor Hierarchy

Model a typed sensor system using inheritance and polymorphism.

**Requirements:**

Base class `Sensor` (abstract) in `sensor.h`:
```
- string name_
- int id_
- pure virtual double read() const = 0
- pure virtual string type() const = 0
- virtual void calibrate() — default implementation prints "Default calibration"
- virtual ~Sensor() — always virtual in a base class
- operator<< overload that calls type() and read()
```

Derived classes, each in their own `.h`/`.cpp`:
- `LidarSensor` — `read()` returns a simulated distance (random float in range), `type()` returns "LIDAR"
- `ImuSensor` — `read()` returns a simulated angular velocity, `type()` returns "IMU", overrides `calibrate()` with IMU-specific behavior
- `WheelOdometrySensor` — `read()` returns a simulated velocity, `type()` returns "ODOMETRY"

`SensorArray` class:
- Holds `std::vector<std::unique_ptr<Sensor>>`
- `addSensor(std::unique_ptr<Sensor> s)`
- `readAll()` — iterates and prints each sensor's output via `operator<<`
- `calibrateAll()` — calls calibrate() on each
- `getSensorById(int id)` — returns raw pointer (non-owning), use `dynamic_cast` to let caller check the concrete type

**Demonstrate in main:**
```cpp
SensorArray array;
array.addSensor(std::make_unique<LidarSensor>("front_lidar", 1));
array.addSensor(std::make_unique<ImuSensor>("imu_0", 2));
array.readAll();

// dynamic_cast demonstration
Sensor* s = array.getSensorById(2);
if (auto* imu = dynamic_cast<ImuSensor*>(s)) {
    imu->calibrate();  // calls IMU-specific override
}
```

**What you're internalizing:** Virtual dispatch (vtable), why base destructors must be virtual, object slicing (try assigning a derived to a base by value and observe what happens), dynamic_cast for safe downcasting. This exact pattern — abstract base + concrete implementations + polymorphic container — is how Nav2 plugins work.

---

## Module 2 — Exception Handling
**Topics:** stack unwinding, nested exceptions, noexcept, exceptions in constructors/destructors

### Assignment 2: Sensor Fault Handling

Extend the sensor hierarchy with a realistic fault model.

**Requirements:**

Exception hierarchy in `sensor_exceptions.h`:
```
SensorException (inherits std::runtime_error)
├── SensorReadError     — sensor returned out-of-range data
├── SensorTimeoutError  — sensor didn't respond
└── CalibrationError    — calibration failed
```

Modify `LidarSensor::read()`:
- Randomly (~20% of calls) throw `SensorReadError` with the sensor name and bad value
- Randomly (~5% of calls) throw `SensorTimeoutError`

Modify `ImuSensor::calibrate()`:
- Accept a `bool force_fail` parameter for testing
- Throw `CalibrationError` when `force_fail = true`

In `SensorArray::readAll()`:
- Catch per-sensor exceptions, log them, and continue reading remaining sensors — one bad sensor shouldn't stop the loop
- Use `std::nested_exception` / `std::throw_with_nested` to wrap a `SensorReadError` inside a higher-level `SensorArrayError` when more than 2 sensors fail in one pass

Add `noexcept` correctly:
- Mark functions that genuinely cannot throw (e.g. simple getters) as `noexcept`
- Leave `read()` and `calibrate()` without it — they can throw

**What you're internalizing:** How RAII (smart pointers) means you don't need cleanup code in catch blocks. Stack unwinding in practice. `noexcept` as a contract, not just an optimization hint. Nested exceptions for wrapping low-level errors with context — a pattern you'll see in production middleware.

---

## Module 3 — File I/O
**Topics:** fstream, error handling, C++17 filesystem, binary I/O

### Assignment 3: Persistent Event Logger

Replace the in-memory event log from Course 1 with a file-backed logger.

**Requirements:**

`EventLogger` class in `event_logger.h`/`.cpp`:
- Constructor takes a `std::filesystem::path` for the log file
- Uses C++17 `std::filesystem` to create the log directory if it doesn't exist
- `log(const std::string& source, const std::string& message)` — writes a timestamped entry to file
- `flush()` — ensures all buffered writes hit disk
- `readBack(int n)` — reads and prints the last n lines from the log file
- `rotateLogs(int max_files)` — if more than `max_files` log files exist in the directory, delete the oldest

Binary log format (separate method `logBinary`):
- Write a fixed-size binary record: `[uint32_t timestamp | uint8_t severity | char[64] message]`
- `parseBinaryLog(const std::filesystem::path& file)` — reads the binary file and prints human-readable records

**What you're internalizing:** `std::fstream` open modes, why you always check stream state after operations, C++17 `std::filesystem` for path manipulation and directory iteration. Binary I/O with fixed-width types — relevant for parsing sensor data formats and ROS bag files.

---

## Module 4 — Templates
**Topics:** function templates, class templates, template argument deduction, explicit specialization, non-type template args, perfect forwarding, variadic templates, type traits

This is the second most important module in Course 2 after OOP. Templates are everywhere in ROS 2 — `rclcpp::Publisher<T>`, `rclcpp::Subscription<T>`, parameter handling, all templated.

### Assignment 4: Generic Data Pipeline

Build a type-safe data pipeline that can carry any sensor reading through a processing chain.

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
```

Explicit specialization for `DataBuffer<bool, N>` — store as a bitset instead of array (space optimization, common interview topic).

**`Pipeline<T>` class template:**
```cpp
template<typename T>
class Pipeline {
    // Holds a DataBuffer<T, 64>
    // addStage(std::function<T(T)> transform) — adds a processing step
    // process(T input) -> T  — runs input through all stages in order
};
```

**Variadic template utility `makeArray`:**
```cpp
template<typename T, typename... Args>
std::array<T, sizeof...(Args)> makeArray(Args&&... args);
// Usage: auto readings = makeArray<double>(1.0, 2.5, 3.7);
```

**Type traits usage:**
```cpp
template<typename T>
void printReading(const T& val) {
    if constexpr (std::is_floating_point_v<T>) {
        std::cout << std::fixed << std::setprecision(4) << val;
    } else if constexpr (std::is_integral_v<T>) {
        std::cout << "int: " << val;
    } else {
        std::cout << val;  // fallback for types with operator<<
    }
}
```

**Demonstrate:**
```cpp
Pipeline<double> lidar_pipeline;
lidar_pipeline.addStage([](double d) { return d * 1000.0; });  // m to mm
lidar_pipeline.addStage([](double d) { return std::clamp(d, 0.0, 5000.0); });

DataBuffer<double, 32> buf;
buf.push(lidar_pipeline.process(2.47));
```

**What you're internalizing:** Template argument deduction, non-type template parameters (N as a compile-time size), perfect forwarding (`T&&` + `std::forward`), variadic templates, `if constexpr` for compile-time branching. This is the foundation for reading ROS 2's publisher/subscriber template machinery.

---

## Module 5 — Lambda Expressions
**Topics:** lambda syntax, captures (by value/reference), generic lambdas, lambdas replacing function objects

### Assignment 5: STL Algorithm Integration

Wire lambdas into your sensor and pipeline systems to do real work with STL algorithms.

**Requirements:**

In `SensorArray`, add:
- `filterSensors(std::function<bool(const Sensor&)> predicate)` — returns vector of non-owning pointers to sensors matching the predicate
- `transformReadings(std::function<double(double)> transform)` — applies transform to all current readings, returns vector of results
- `sortSensorsBy(std::function<bool(const Sensor&, const Sensor&)> comparator)` — sorts internal vector in-place

Demonstrate all three with lambdas:
```cpp
// Filter to only LIDAR sensors
auto lidars = array.filterSensors([](const Sensor& s) {
    return s.type() == "LIDAR";
});

// Convert all readings from meters to cm
auto cm_readings = array.transformReadings([](double d) { return d * 100.0; });

// Sort by sensor ID
array.sortSensorsBy([](const Sensor& a, const Sensor& b) {
    return a.id() < b.id();
});
```

Add to `Pipeline<T>`:
- `addStage` should accept any callable — function pointer, lambda, or `std::function` — using a template:
```cpp
template<typename F>
void addStage(F&& f);  // perfect forwarding of callable
```

Mutable lambda demo — write a lambda that captures a counter by value, increments it, and explain why `mutable` is needed:
```cpp
int dropped{0};
auto filter = [dropped](double val) mutable -> std::optional<double> {
    if (val < 0.0) { ++dropped; return std::nullopt; }
    return val;
};
```

**What you're internalizing:** Lambda captures and their lifetime implications (capturing by reference when the lambda outlives the scope = bug), generic lambdas as an alternative to function templates, why `std::function` has overhead vs direct lambda passing. All three are interview topics.

---

## Module 6 — STL Containers & Algorithms
**Topics:** vector, map, unordered_map, list, set, iterators, algorithms, Big-O awareness

### Assignment 6: Task Scheduler

Build a priority-aware task scheduler using STL containers — the kind of component that manages behavior execution in a mobile robot.

**`RobotTask` struct:**
```cpp
struct RobotTask {
    int id;
    std::string name;
    int priority;           // higher = more urgent
    double estimated_cost;  // seconds
    bool operator<(const RobotTask& other) const;  // for use in ordered containers
};
```

**`TaskScheduler` class:**
```cpp
class TaskScheduler {
    std::priority_queue<RobotTask> ready_queue_;       // highest priority first
    std::unordered_map<int, RobotTask> all_tasks_;     // O(1) lookup by ID
    std::map<int, std::vector<int>> dependencies_;     // task_id -> list of dependency IDs
    std::set<int> completed_;                          // O(log n) membership test

public:
    void addTask(RobotTask task);
    void addDependency(int task_id, int depends_on_id);
    std::optional<RobotTask> nextReady();   // returns highest-priority task with all deps met
    void markComplete(int task_id);
    void printQueue() const;
    std::vector<RobotTask> getByPriorityRange(int min_p, int max_p) const;  // use std::copy_if
};
```

**Algorithm requirements — use STL algorithms explicitly, not hand-written loops:**
- `std::sort` to sort a snapshot of tasks by estimated cost
- `std::find_if` to locate a task by name
- `std::accumulate` to compute total estimated cost of pending tasks
- `std::copy_if` for `getByPriorityRange`
- `std::transform` to extract task names into a `vector<string>`

**Demonstrate Big-O awareness** — add a comment block above each container choice explaining why you chose it:
```cpp
// std::unordered_map: O(1) average lookup by ID — used for frequent ID-based access
// std::set: O(log n) insert/lookup — used for completed set where membership check is frequent
// std::priority_queue: O(log n) push/pop — appropriate for scheduling where we always want max priority
```

**What you're internalizing:** Container selection is an interview staple — knowing why `unordered_map` beats `map` for lookups, when `set` is appropriate vs `vector + sort`, why `priority_queue` exists. The STL algorithm practice here also directly mirrors what you'll write in Nav2 plugin code.

---

## Course 2 Capstone: RobotMiddleware

Wire everything from Course 2 into a `RobotMiddleware` system that integrates with your `RobotRuntime` from Course 1.

**Architecture:**
```
RobotMiddleware
├── SensorArray          (Assignment 1 — polymorphic sensors)
├── EventLogger          (Assignment 3 — file-backed logging)
├── Pipeline<double>     (Assignment 4 — generic processing)
├── TaskScheduler        (Assignment 6 — STL task management)
└── RobotRuntime*        (non-owning pointer back to Course 1 runtime)
```

**Full requirements:**

`RobotMiddleware` class:
- Constructor takes a `std::filesystem::path` for log directory and a `RobotRuntime*`
- Initializes all subsystems
- `startSession()` — logs session start, calibrates all sensors
- `runCycle()` — reads all sensors, processes readings through pipeline, logs anomalies, checks for ready tasks and executes the highest priority one
- `shutdown()` — flushes logger, logs final stats

Sensor pipeline in `runCycle()`:
- Read all sensors via `SensorArray`
- Filter out failed reads with a lambda
- Pass valid readings through `Pipeline<double>` (unit conversion + clamping)
- Use `std::accumulate` to compute mean of valid readings
- If mean is outside a threshold, log a warning via `EventLogger`

Task integration:
- Pre-load 5 tasks with varying priorities and dependencies (e.g. "check_battery" must complete before "navigate_to_goal")
- Each `runCycle()` call executes the next ready task if one exists
- Completed tasks logged to file

**main.cpp:**
```cpp
auto runtime = std::make_unique<RobotRuntime>("Atlas", 1);
RobotMiddleware middleware{std::filesystem::path{"logs/"}, runtime.get()};

middleware.startSession();
for (int i = 0; i < 10; ++i) {
    middleware.runCycle();
}
middleware.shutdown();
```

**What you're proving:** You can design a multi-component C++ system with clean ownership semantics, polymorphic interfaces, generic data structures, file I/O, and STL-idiomatic code. This is directly analogous to the architecture of a Nav2 plugin or a ROS 2 lifecycle node. The code you write here is something you could legitimately reference in an interview.

---

## Suggested Pacing

| Assignment | Estimated Time |
|---|---|
| A1: Sensor Hierarchy | 4–5 hrs |
| A2: Sensor Fault Handling | 2–3 hrs |
| A3: Persistent Event Logger | 3–4 hrs |
| A4: Generic Data Pipeline | 5–6 hrs |
| A5: STL Algorithm Integration | 3–4 hrs |
| A6: Task Scheduler | 4–5 hrs |
| Capstone: RobotMiddleware | 6–8 hrs |
| **Total** | **~27–35 hrs** |

Assignment 4 (Templates) is the one most people underestimate. Budget extra time and don't move on until perfect forwarding and variadic templates genuinely make sense — they'll come up in interviews and you'll read them constantly in ROS 2 source.