cmake_minimum_required(VERSION 3.16)

project(wheel_speed_signal_checker LANGUAGES CXX)

add_executable(wheel_speed_check main.cpp)

target_compile_features(wheel_speed_check PRIVATE cxx_std_17)
set_target_properties(wheel_speed_check PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(CMAKE_CXX_COMPILER_ID MATCHES "^(AppleClang|Clang|GNU)$")
    target_compile_options(wheel_speed_check PRIVATE -Wall -Wextra -Wpedantic)
endif()
