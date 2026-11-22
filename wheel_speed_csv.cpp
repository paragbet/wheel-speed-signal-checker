#include "wheel_speed_csv.hpp"

#include <charconv>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <system_error>

namespace {

constexpr std::string_view kExpectedHeader = "timestamp_ms,wheel_speed_kmh";

bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

bool all_digits(std::string_view text)
{
    if (text.empty()) {
        return false;
    }
    for (char c : text) {
        if (!is_digit(c)) {
            return false;
        }
    }
    return true;
}

// Digits only, so signs, spaces, and suffixes are rejected before conversion.
std::optional<std::int64_t> parse_timestamp(std::string_view text)
{
    if (!all_digits(text)) {
        return std::nullopt;
    }
    std::int64_t value = 0;
    const char* const end = text.data() + text.size();
    const auto [ptr, ec] = std::from_chars(text.data(), end, value);
    if (ec != std::errc{} || ptr != end) {
        return std::nullopt; // Includes std::errc::result_out_of_range.
    }
    return value;
}

// Accepts "<digits>" or "<digits>.<digits>".
std::optional<double> parse_speed(std::string_view text)
{
    const std::size_t dot = text.find('.');
    const std::string_view whole = text.substr(0, dot);
    if (!all_digits(whole)) {
        return std::nullopt;
    }
    if (dot != std::string_view::npos && !all_digits(text.substr(dot + 1))) {
        return std::nullopt;
    }

    // The text is already validated, so strtod only converts it. A copy is
    // needed because strtod requires a null-terminated string.
    const std::string copy(text);
    char* end = nullptr;
    const double value = std::strtod(copy.c_str(), &end);
    if (end != copy.c_str() + copy.size() || !std::isfinite(value)) {
        return std::nullopt;
    }
    return value;
}

void strip_trailing_cr(std::string& line)
{
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
}

std::string location(const std::string& path, std::size_t line_number)
{
    return path + ":" + std::to_string(line_number) + ": ";
}

} // namespace

std::optional<WheelSpeedSample> parse_wheel_speed_row(std::string_view row)
{
    const std::size_t comma = row.find(',');
    if (comma == std::string_view::npos) {
        return std::nullopt;
    }

    const std::string_view timestamp_text = row.substr(0, comma);
    const std::string_view speed_text = row.substr(comma + 1);
    // A second comma means an extra column; parse_speed would reject it too,
    // but checking here keeps the two-field rule explicit.
    if (speed_text.find(',') != std::string_view::npos) {
        return std::nullopt;
    }

    const auto timestamp = parse_timestamp(timestamp_text);
    const auto speed = parse_speed(speed_text);
    if (!timestamp || !speed) {
        return std::nullopt;
    }
    return WheelSpeedSample{*timestamp, *speed};
}

std::vector<WheelSpeedSample> read_wheel_speed_csv(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error(path + ": cannot open file");
    }

    std::string line;
    std::size_t line_number = 1;
    if (!std::getline(file, line)) {
        if (file.bad()) {
            throw std::runtime_error(path + ": cannot read file");
        }
        throw std::runtime_error(location(path, line_number) +
                                 "invalid header: file is empty, expected '" +
                                 std::string(kExpectedHeader) + "'");
    }
    strip_trailing_cr(line);
    if (line != kExpectedHeader) {
        throw std::runtime_error(location(path, line_number) +
                                 "invalid header '" + line + "', expected '" +
                                 std::string(kExpectedHeader) + "'");
    }

    std::vector<WheelSpeedSample> samples;
    while (std::getline(file, line)) {
        ++line_number;
        strip_trailing_cr(line);
        const auto sample = parse_wheel_speed_row(line);
        if (!sample) {
            throw std::runtime_error(location(path, line_number) +
                                     "invalid data row '" + line + "'");
        }
        samples.push_back(*sample);
    }

    if (file.bad()) {
        throw std::runtime_error(path + ": cannot read file");
    }
    return samples;
}
