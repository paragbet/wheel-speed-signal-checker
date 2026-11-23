cmake_minimum_required(VERSION 3.16)

project(wheel_speed_signal_checker LANGUAGES CXX)

# Writes build/compile_commands.json so editors use the same flags
# (including -std=c++17) as the real build.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Applies the project's C++17 and warning settings to a target.
function(wheel_speed_configure_target target)
    target_compile_features(${target} PRIVATE cxx_std_17)
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
    if(CMAKE_CXX_COMPILER_ID MATCHES "^(AppleClang|Clang|GNU)$")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

add_executable(wheel_speed_check main.cpp)
wheel_speed_configure_target(wheel_speed_check)

add_library(wheel_speed_csv STATIC wheel_speed_csv.cpp)
target_include_directories(wheel_speed_csv PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_features(wheel_speed_csv PUBLIC cxx_std_17)
wheel_speed_configure_target(wheel_speed_csv)

add_library(wheel_speed_checker STATIC wheel_speed_checker.cpp)
target_include_directories(wheel_speed_checker PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_features(wheel_speed_checker PUBLIC cxx_std_17)
wheel_speed_configure_target(wheel_speed_checker)

# The application uses the reader and the checker.
target_link_libraries(wheel_speed_check PRIVATE wheel_speed_csv wheel_speed_checker)

enable_testing()

add_executable(wheel_speed_csv_tests tests/wheel_speed_csv_tests.cpp)
target_link_libraries(wheel_speed_csv_tests PRIVATE wheel_speed_csv)
wheel_speed_configure_target(wheel_speed_csv_tests)

# Absolute fixture paths come from the source tree, so CTest finds them
# wherever the repository is cloned and whichever build directory is used.
add_test(
    NAME wheel_speed_csv_tests
    COMMAND wheel_speed_csv_tests
        ${PROJECT_SOURCE_DIR}/samples/normal.csv
        ${PROJECT_SOURCE_DIR}/tests/fixtures/invalid.csv
)

add_executable(wheel_speed_checker_tests tests/wheel_speed_checker_tests.cpp)
target_link_libraries(wheel_speed_checker_tests PRIVATE wheel_speed_checker)
wheel_speed_configure_target(wheel_speed_checker_tests)

add_test(NAME wheel_speed_checker_tests COMMAND wheel_speed_checker_tests)

# Integration tests run the real program with command-line arguments.
# Two tests are needed per case because PASS_REGULAR_EXPRESSION makes CTest
# ignore the exit code: one test checks the exit code, the other the output.
add_test(NAME wheel_speed_check_runs
    COMMAND wheel_speed_check ${PROJECT_SOURCE_DIR}/samples/normal.csv)

add_test(NAME wheel_speed_check_summary
    COMMAND wheel_speed_check ${PROJECT_SOURCE_DIR}/samples/normal.csv)
set_tests_properties(wheel_speed_check_summary PROPERTIES
    PASS_REGULAR_EXPRESSION "Samples checked: 3\nSamples over limit: 2"
)

# Adds two tests for a case where wheel_speed_check must fail:
#   <name>_fails   passes if the exit code is non-zero (WILL_FAIL).
#   <name>_message passes if the output contains expected_message.
# Any arguments after expected_message (ARGN) are passed to the program.
function(add_wheel_speed_check_failure_test name expected_message)
    add_test(NAME ${name}_fails COMMAND wheel_speed_check ${ARGN})
    set_tests_properties(${name}_fails PROPERTIES WILL_FAIL TRUE)

    add_test(NAME ${name}_message COMMAND wheel_speed_check ${ARGN})
    set_tests_properties(${name}_message PROPERTIES
        PASS_REGULAR_EXPRESSION "${expected_message}"
    )
endfunction()

add_wheel_speed_check_failure_test(wheel_speed_check_no_file
    "Usage: wheel_speed_check <csv-file>")

add_wheel_speed_check_failure_test(wheel_speed_check_too_many_arguments
    "Usage: wheel_speed_check <csv-file>"
    ${PROJECT_SOURCE_DIR}/samples/normal.csv extra)

add_wheel_speed_check_failure_test(wheel_speed_check_missing_file
    "missing.csv: cannot open file"
    ${PROJECT_SOURCE_DIR}/samples/missing.csv)
