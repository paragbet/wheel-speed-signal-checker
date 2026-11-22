#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "wheel_speed_sample.hpp"

// Parses one CSV data row of the form "<timestamp_ms>,<wheel_speed_kmh>".
// The row must not contain a line terminator. Returns an empty optional if
// the row is invalid.
std::optional<WheelSpeedSample> parse_wheel_speed_row(std::string_view row);

// Reads a wheel-speed CSV file. The first line must be exactly
// "timestamp_ms,wheel_speed_kmh". Accepts LF and CRLF line endings.
// Returns the samples in file order. Throws std::runtime_error if the file
// cannot be read, the header is invalid, or any data row is invalid.
std::vector<WheelSpeedSample> read_wheel_speed_csv(const std::string& path);
