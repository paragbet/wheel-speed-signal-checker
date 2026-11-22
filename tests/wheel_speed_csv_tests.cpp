// Dependency-free tests for the wheel-speed CSV reader.
//
// Usage: wheel_speed_csv_tests <valid.csv> <invalid.csv>
// CTest passes absolute fixture paths, so the tests work from any clone.

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include "wheel_speed_csv.hpp"

namespace {

int g_failures = 0;

void check(bool condition, const std::string& message, int line)
{
    if (!condition) {
        ++g_failures;
        std::cerr << "FAIL (" << __FILE__ << ":" << line << "): " << message << "\n";
    }
}

#define CHECK(condition, message) check((condition), (message), __LINE__)

bool contains(const std::string& text, const std::string& part)
{
    return text.find(part) != std::string::npos;
}

// Runs read_wheel_speed_csv and returns the exception message, or an empty
// string if nothing was thrown.
std::string read_error(const std::string& path)
{
    try {
        read_wheel_speed_csv(path);
    } catch (const std::runtime_error& error) {
        return error.what();
    }
    return {};
}

void test_valid_row()
{
    const auto sample = parse_wheel_speed_row("100,35.5");
    CHECK(sample.has_value(), "\"100,35.5\" should parse");
    if (sample) {
        CHECK(sample->timestamp_ms == 100,
              "timestamp: expected 100, got " + std::to_string(sample->timestamp_ms));
        CHECK(sample->wheel_speed_kmh == 35.5,
              "speed: expected 35.5, got " + std::to_string(sample->wheel_speed_kmh));
    }
}

void test_missing_field_rejected()
{
    CHECK(!parse_wheel_speed_row("100"), "\"100\" (no comma) should be rejected");
    CHECK(!parse_wheel_speed_row("100,"), "\"100,\" (empty speed) should be rejected");
    CHECK(!parse_wheel_speed_row(",35"), "\",35\" (empty timestamp) should be rejected");
}

void test_valid_file(const std::string& path)
{
    try {
        const auto samples = read_wheel_speed_csv(path);
        CHECK(samples.size() == 3,
              "expected 3 samples, got " + std::to_string(samples.size()));
        if (samples.size() == 3) {
            CHECK(samples[0].timestamp_ms == 0 && samples[0].wheel_speed_kmh == 20.0,
                  "sample 0 should be {0, 20}");
            CHECK(samples[1].timestamp_ms == 100 && samples[1].wheel_speed_kmh == 35.5,
                  "sample 1 should be {100, 35.5}");
            CHECK(samples[2].timestamp_ms == 200 && samples[2].wheel_speed_kmh == 42.0,
                  "sample 2 should be {200, 42}");
        }
    } catch (const std::exception& error) {
        CHECK(false, std::string("valid file threw: ") + error.what());
    }
}

void test_invalid_file_line_number(const std::string& path)
{
    const std::string message = read_error(path);
    CHECK(!message.empty(), "invalid fixture should throw std::runtime_error");
    CHECK(contains(message, path), "error should name the file, got: " + message);
    CHECK(contains(message, path + ":3:"),
          "error should report line 3, got: " + message);
}

void test_missing_file()
{
    const std::string path = "this_file_does_not_exist.csv";
    const std::string message = read_error(path);
    CHECK(!message.empty(), "missing file should throw std::runtime_error");
    CHECK(contains(message, path), "error should name the file, got: " + message);
    CHECK(contains(message, "cannot open"),
          "error should say the file cannot be opened, got: " + message);
}

} // namespace

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " <valid.csv> <invalid.csv>\n";
        return 2;
    }
    const std::string valid_path = argv[1];
    const std::string invalid_path = argv[2];

    test_valid_row();
    test_missing_field_rejected();
    test_valid_file(valid_path);
    test_invalid_file_line_number(invalid_path);
    test_missing_file();

    // Add your own test here.

    const auto result = parse_wheel_speed_row("100, 35extra");
    if (result.has_value()) {
        std::cerr << "FAIL ( " << __FILE__ << ":" << __LINE__ << " ) : " << "\"100, 35extra\" (trailing text in speed)should be rejected\n";
        return 1;
    }

    if (g_failures != 0) {
        std::cerr << g_failures << " check(s) failed\n";
        return 1;
    }

    std::cout << "All wheel-speed CSV tests passed\n";
    return 0;
}
