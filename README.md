# Wheel-Speed Signal Checker

A small command-line tool that reads recorded wheel-speed samples from a CSV
file and reports every sample that is faster than a speed limit.

It is meant for people who work with recorded vehicle signals, for example
someone testing or reviewing logged data, who want a quick check of whether a
wheel-speed signal ever went above a chosen limit.

For each sample, the tool compares the wheel speed with the limit. A sample is
**over the limit** only when its speed is strictly greater than the limit. A
sample that is exactly equal to the limit is not reported.

The files in `samples/` and `tests/fixtures/` contain **synthetic data** made
up for examples and tests. They are not recordings from a real vehicle.

## Required tools

- A C++ compiler that supports **C++17**, for example Apple Clang (from the
  Xcode Command Line Tools) on macOS, or GCC on Linux.
- **CMake 3.16 or newer** (the minimum set in `CMakeLists.txt`).
- CTest, which is installed together with CMake.
- LLDB, only if you want to use the debugger (see below). On macOS it comes
  with the Xcode Command Line Tools.

No other libraries are needed.

## Build and test

Run these commands from the repository root (the folder that contains
`CMakeLists.txt`).

```
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

- `cmake -S . -B build` **configures** the project. `-S .` means the source
  code is in the current folder. `-B build` means the build files go in a
  folder named `build`.
- `cmake --build build` **compiles** the program and the test programs.
  The program is created at `build/wheel_speed_check`.
- `ctest --test-dir build` **runs all tests**. Expect
  `100% tests passed out of 15`. Add `--output-on-failure` to see a failing
  test's output.

## Running the checker

```
./build/wheel_speed_check <csv-file> [limit-kmh]
```

- `<csv-file>` is required. It is the path to the CSV file to check.
- `[limit-kmh]` is optional. It is the speed limit in km/h. If you leave it
  out, the default limit is **35 km/h**.

### With the default limit (35 km/h)

```
./build/wheel_speed_check samples/normal.csv
```

Expected output for `samples/normal.csv`:

```
Over limit at 100 ms: 35.5 km/h
Over limit at 200 ms: 42 km/h
Samples checked: 3
Samples over limit: 2
```

The sample at 0 ms (20 km/h) is not over 35 km/h, so it is not listed.

### With a limit you choose

```
./build/wheel_speed_check samples/normal.csv 40
```

Expected output:

```
Over limit at 200 ms: 42 km/h
Samples checked: 3
Samples over limit: 1
```

The limit can be a whole number or a decimal number, such as `40` or `35.5`.
It must be zero or more. Text such as `fast`, a negative number, or extra
text after the number is rejected with an error.

### Output

- One `Over limit at <timestamp> ms: <speed> km/h` line for each sample over
  the limit, in the same order as the file.
- Then two summary lines: how many samples were checked, and how many were
  over the limit.

If no sample is over the limit, only the two summary lines are printed.

## CSV file format

Example (`samples/normal.csv`):

```
timestamp_ms,wheel_speed_kmh
0,20
100,35.5
200,42
```

| Column            | Unit                  | Accepted values                                                  |
| ----------------- | --------------------- | ---------------------------------------------------------------- |
| `timestamp_ms`    | milliseconds          | Whole numbers of 0 or more, digits only (for example `0`, `100`). |
| `wheel_speed_kmh` | kilometres per hour   | Numbers of 0 or more, digits with an optional decimal part (for example `20`, `35.5`). |

Rules:

- The first line must be exactly `timestamp_ms,wheel_speed_kmh`.
- Every following line must have exactly two values separated by one comma.
- Spaces, `+` or `-` signs, and exponents (such as `1e2`) are not accepted.
  A decimal point needs digits on both sides (`35.5` is accepted, `.5` and
  `5.` are not).
- Empty lines are not accepted, including an empty line at the end of the
  file. A single line break after the last row is fine.
- Both LF (macOS/Linux) and CRLF (Windows) line endings are accepted.
- A file with only the header line is valid. It reports
  `Samples checked: 0`.
- The tool does not check that timestamps are in order. Samples are checked
  in the order they appear in the file.

### Invalid input

The tool stops at the first problem. It prints one message to the error
output (stderr), prints no over-limit lines or summary, and exits with
status 1. Wrong arguments print the usage line. Other problems print a line
that starts with `Error:`. Errors inside the CSV file name the file and the
line number.

| Problem                         | Example message                                                                         |
| ------------------------------- | --------------------------------------------------------------------------------------- |
| No CSV path, or too many words  | `Usage: wheel_speed_check <csv-file> [limit-kmh]`                                       |
| Limit is not a number           | `Error: speed limit 'fast' is not a number`                                             |
| Limit is negative               | `Error: speed limit '-5' must not be negative`                                          |
| File cannot be opened           | `Error: samples/missing.csv: cannot open file`                                          |
| Empty file                      | `Error: <file>:1: invalid header: file is empty, expected 'timestamp_ms,wheel_speed_kmh'` |
| Wrong header                    | `Error: <file>:1: invalid header 'time,speed', expected 'timestamp_ms,wheel_speed_kmh'` |
| Invalid data row                | `Error: tests/fixtures/invalid.csv:3: invalid data row '100,-35'`                       |

## Exit status

The exit status is the number a program returns to the shell when it ends.
You can see it by running `echo $?` straight after the program.

| Exit status | Meaning                                                                                       |
| ----------- | --------------------------------------------------------------------------------------------- |
| `0`         | The file was read and checked. This is `0` even when samples are over the limit.             |
| `1`         | The check did not run: wrong arguments, invalid limit, or a file that cannot be read or is invalid. |

To find out whether any sample was over the limit, read the
`Samples over limit:` line, not the exit status.

## Automatic checks on GitHub (Linux build and test)

The workflow in `.github/workflows/build-and-test.yml` runs on GitHub Actions
for every pull request to `main` and every push to `main`. It:

1. Uses an Ubuntu 24.04 Linux machine with the GCC compiler, so the code is
   also checked with a different compiler than Apple Clang on macOS.
2. Configures the project in Debug mode, builds it, and runs all CTest tests.

If the build or any test fails, the check is shown as failed on the pull
request. The result appears in the **Checks** section of the pull request and
in the **Actions** tab of the repository.

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
