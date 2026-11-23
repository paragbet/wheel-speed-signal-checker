# wheel-speed-signal-checker

## Debugging with LLDB

LLDB is the debugger that comes with Apple's command-line tools. It can pause
the program at a chosen function so you can look at the values it is using.

In the steps below, lines starting with `%` run in the **shell** (your normal
terminal), and lines starting with `(lldb)` run **inside LLDB**, after it shows
its `(lldb)` prompt. Do not type the `%` or `(lldb)` prefix itself.

### 1. Build with debug information (shell)

```
% cmake -S . -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
% cmake --build build
```

`-DCMAKE_BUILD_TYPE=Debug` tells the compiler to keep debug information
(source file names, line numbers, and variable names) and not to optimize
the code. Without it, LLDB may not show variables or the correct lines. The
setting is stored in the `build` directory, so later builds keep using it.

### 2. Start LLDB (shell)

Run this from the repository root, because `samples/normal.csv` is a relative
path:

```
% lldb ./build/wheel_speed_check
```

Expect a `(lldb)` prompt. The program is loaded but not running yet.

### 3. Set a breakpoint (LLDB)

```
(lldb) breakpoint set --name WheelSpeedChecker::is_over_limit
```

A breakpoint makes the program pause every time that function is called.
Expect `Breakpoint 1: ... at wheel_speed_checker.cpp:34`.

### 4. Run with arguments (LLDB)

```
(lldb) run samples/normal.csv 40
```

The words after `run` become the program's command-line arguments. Expect
the program to stop at the breakpoint, with an arrow `->` pointing at
`return sample.wheel_speed_kmh > limit_kmh;`.

### 5. Inspect variables (LLDB)

```
(lldb) frame variable sample
(lldb) frame variable this->limit_kmh
```

`frame variable` prints variables from the function where the program is
paused. `this->limit_kmh` is the private limit stored inside the checker
object. On the first stop, expect `timestamp_ms = 0, wheel_speed_kmh = 20`
and `this->limit_kmh = 40`.

### 6. Show the call stack (LLDB)

```
(lldb) bt
```

`bt` (backtrace) lists the chain of function calls that led here, newest
first. Expect `frame #0` to be `WheelSpeedChecker::is_over_limit` and
`frame #1` to be `main` in `main.cpp`, the line inside the checking loop.

### 7. Continue and quit (LLDB)

```
(lldb) continue
(lldb) quit
```

`continue` resumes the program until the next breakpoint stop. Expect the
next stop to show the sample at 100 ms (35.5 km/h). Each `continue` moves to
the next sample. After the last sample, expect the program's normal output and
`Process ... exited with status = 0`.

`quit` leaves LLDB and returns to the shell. If the program is still paused,
LLDB asks whether to stop it. Answer `y`.
