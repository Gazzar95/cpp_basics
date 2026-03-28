# Packt Modern C++ — Course 1 Assignments
## Foundations of Modern C++

**Theme:** You're rewriting your C robot runtime in C++, module by module, learning the C++ way to do things you already know how to do in C. By the end of Course 1 you'll have a working `RobotRuntime` C++ program that replaces your C version — same functionality, modern C++ internals.

**Ground rules:**
- Compile with `g++ -std=c++17 -Wall -Wextra` throughout
- One `.cpp` / `.h` pair per logical component (same multi-file discipline as your C project)
- Don't use `malloc`/`free` — learn the C++ equivalents
- Each assignment builds on the last; don't throw code away

---

## Module 1 — Setup
*Skip. You're on Ubuntu 22.04 with g++ already available. Confirm with `g++ --version` and move on.*

---

## Module 2 — Basic Language Facilities
**Topics:** types, I/O, functions, uniform initialization, pointers, references, compilation

### Assignment 2: Hello, C++ Runtime

Rewrite just the entry point and command parser from your C project in C++.

**Requirements:**
- `main.cpp` reads commands from stdin in a loop until `QUIT`
- Use `std::string` and `std::cin` instead of `char[]` and `scanf`
- Write a `parseCommand(const std::string& line)` function that returns the command token and an optional numeric argument
- Use uniform initialization (`{}`) everywhere — no `= value` style init
- Use references in function signatures instead of pointers where appropriate

**Deliverable:** Program compiles and responds to `SAMPLE`, `STATS`, `PUSH`, `POP`, `LOG`, `QUIT` with placeholder `std::cout` responses (e.g. "SAMPLE received: 10").

**What you're internalizing:** `std::string` vs `char*`, `std::cin/cout` vs `scanf/printf`, reference vs pointer semantics, the compilation model in C++ (same as C but now with headers that matter more).

---

## Module 3 — Dynamic Memory Allocation
**Topics:** `new`/`delete`, heap arrays, 2D arrays

### Assignment 3: Sensor Ring Buffer

Reimplement your sensor ring buffer from the C project using C++ `new`/`delete`.

**Requirements:**
- Class `SensorBuffer` in `sensor_buffer.h` / `sensor_buffer.cpp`
- Constructor takes `capacity` as argument, allocates buffer on the heap with `new`
- Destructor frees with `delete[]`
- Methods: `push(double value)`, `stats()` (prints min, max, mean), `isFull()`, `isEmpty()`
- In `main.cpp`, instantiate with `SensorBuffer buffer{10};` (uniform init)
- Handle the edge cases: push to full buffer overwrites oldest (ring behavior)

**What you're internalizing:** `new`/`delete` vs `malloc`/`free`, constructor/destructor lifecycle, why you need `delete[]` for arrays. You'll also start to see why smart pointers exist — you're managing this manually on purpose first.

---

## Module 4 — Object-Oriented Programming
**Topics:** classes, constructors, destructors, access specifiers, `this`

### Assignment 4: RobotRuntime Class

Wrap the whole program in a `RobotRuntime` class.

**Requirements:**
- `RobotRuntime` class in `robot_runtime.h` / `robot_runtime.cpp`
- Private members: `SensorBuffer`, a name string, a session ID (just an int)
- Constructor: `RobotRuntime(const std::string& name, int session_id)`
- Public method: `run()` — contains the command loop from Assignment 2
- Public method: `printStatus()` — prints name, session ID, buffer stats
- Explicit destructor that prints "Runtime shutting down: [name]" on exit
- No public data members — everything goes through methods

**What you're internalizing:** encapsulation, constructor initialization lists, access control, object lifetime tied to scope.

---

## Module 5 — Move Semantics & Smart Pointers
**Topics:** lvalue/rvalue, move constructor, move assignment, `unique_ptr`, `shared_ptr`, `weak_ptr`

### Assignment 5: Smart Pointer Refactor

This is the most important assignment in Course 1. Refactor `SensorBuffer` and `RobotRuntime` to use smart pointers instead of raw `new`/`delete`.

**Requirements:**
- Replace raw `new double[]` in `SensorBuffer` with `std::unique_ptr<double[]>`
- Remove the manual destructor from `SensorBuffer` — the unique_ptr handles it
- In `RobotRuntime`, hold `SensorBuffer` as `std::unique_ptr<SensorBuffer>`, created with `std::make_unique`
- Add a move constructor and move assignment operator to `SensorBuffer`
- Demonstrate move semantics: create a `SensorBuffer`, move it into a `RobotRuntime`, show the original is empty after the move
- Add a second class `SharedLogger` that holds a `std::shared_ptr` to a log buffer — demonstrate two objects sharing ownership and the buffer surviving until both are destroyed

**What you're internalizing:** This is where C++ diverges hard from C. You're learning that memory ownership is expressed in types, not conventions. `unique_ptr` = sole owner. `shared_ptr` = shared ownership. Move semantics = transferring ownership without copying. This mental model is load-bearing for everything in robotics C++ — ROS 2 nodes, Nav2 plugins, everything uses these patterns.

**Concrete test to write:**
```cpp
// This should NOT compile — unique_ptr is not copyable
// auto buf2 = buf1;  // intentionally leave this commented with explanation

// This should work — move transfers ownership
SensorBuffer buf1{10};
buf1.push(3.14);
SensorBuffer buf2 = std::move(buf1);
// buf1 is now empty/invalid, buf2 owns the data
```

---

## Module 6 — Operator Overloading & Type Conversion
**Topics:** overloading `<<`, `[]`, `==`, conversion operators

### Assignment 6: Operator Overloads

Add operator overloads to your existing classes.

**Requirements:**
- Overload `operator<<` for `SensorBuffer` so `std::cout << buffer` prints its contents
- Overload `operator[]` on `SensorBuffer` for indexed read access
- Overload `operator==` on `SensorBuffer` to compare two buffers by their contents
- Add an explicit conversion operator `explicit operator bool()` that returns `true` if the buffer has data
- Add a `CommandEvent` struct and overload `operator<<` for it so events print cleanly

**What you're internalizing:** Idiomatic C++ uses operators heavily — the STL is built on this. `operator<<` for logging, `operator[]` for containers, `explicit operator bool` for null-checks. You'll see all of these in ROS 2 and Nav2 source.

---

## Module 7 — Exception Handling
**Topics:** try/catch, custom exceptions, stack unwinding

### Assignment 7: Robust Error Handling

Add exception-based error handling to `RobotRuntime`.

**Requirements:**
- Create a custom exception class `RobotError` that inherits from `std::runtime_error`
- Subclass it: `SensorOverflowError`, `InvalidCommandError`
- Throw `SensorOverflowError` if a sensor value is out of range (e.g. > 1000.0)
- Throw `InvalidCommandError` if an unrecognized command is entered
- Wrap the command loop in `RobotRuntime::run()` with try/catch — catch specific errors, log them, and continue running (don't crash on bad input)
- Add a catch-all at the outermost level in `main.cpp`

**What you're internalizing:** Exception hierarchies, catch ordering (specific before general), the difference between recoverable and fatal errors. You'll also observe RAII in action — smart pointers clean up correctly even when exceptions are thrown, without any extra code from you.

---

## Module 8 — Course 1 Capstone
**Topics:** Everything from Course 1

### Capstone: Full RobotRuntime in C++

Complete and polish the `RobotRuntime` program. This is a direct C++ successor to your C project.

**Full feature requirements:**

**Sensor subsystem:**
- `SensorBuffer` using `unique_ptr<double[]>`, ring buffer behavior, `operator<<`, `operator[]`, `explicit operator bool`
- `stats()` method: min, max, mean, std deviation

**Command stack:**
- Reimplement your FILO command stack using `std::stack<std::string>` from the STL (stop managing this manually)
- `PUSH <cmd>` and `POP` still work as before

**Event log:**
- Reimplement your linked list event log using `std::list<CommandEvent>` from the STL
- `CommandEvent` struct with timestamp (just an incrementing int for now), command string, operator<< overload
- `LOG` command prints all events

**RobotRuntime class:**
- Holds all subsystems as private members via smart pointers
- `run()` command loop with full exception handling
- `printStatus()` prints name, session, buffer stats, stack depth, log size
- Move constructible (you should be able to `std::move` a runtime)

**main.cpp:**
- Instantiate with `std::make_unique<RobotRuntime>("Atlas", 1)`
- Call `->run()`
- Outer try/catch for fatal errors

**What you're proving:** You can translate your C mental model into idiomatic modern C++. You own memory correctly through types, not manual tracking. Your classes have clean lifetimes. Error conditions don't crash the program. This is the baseline expected of a junior C++ robotics candidate.

---

## Suggested Pacing

| Assignment | Estimated Time |
|---|---|
| A2: Command Parser | 2–3 hrs |
| A3: Sensor Ring Buffer | 3–4 hrs |
| A4: RobotRuntime Class | 2–3 hrs |
| A5: Smart Pointer Refactor | 4–5 hrs |
| A6: Operator Overloads | 2–3 hrs |
| A7: Exception Handling | 2–3 hrs |
| Capstone | 4–6 hrs |
| **Total** | **~20–27 hrs** |

This runs roughly in parallel with the lecture content — do the assignment after finishing each module's videos, before moving to the next module.