// Dependency-free tests for WheelSpeedChecker.
//
// Usage: wheel_speed_checker_tests

#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

#include "wheel_speed_checker.hpp"

using namespace std;

namespace {

int g_failures = 0;

void check(bool condition, const string& message, int line)
{
    if (!condition) {
        ++g_failures;
        cerr << "FAIL (" << __FILE__ << ":" << line << "): " << message << "\n";
    }
}

#define CHECK(condition, message) check((condition), (message), __LINE__)

// Returns true if constructing a checker with this limit throws
// std::invalid_argument.
bool constructor_rejects(double speed_limit_kmh)
{
    try {
        WheelSpeedChecker checker(speed_limit_kmh);
    } catch (const invalid_argument&) {
        return true;
    }
    return false;
}

// Input: limit 50, sample speed 30. Expected: not over the limit.
void test_speed_below_limit()
{
    const WheelSpeedChecker checker(50.0);
    const WheelSpeedSample sample{100, 30.0};
    CHECK(!checker.is_over_limit(sample), "30 km/h should not be over a 50 km/h limit");
}

// Input: limit 50, sample speed 70. Expected: over the limit.
void test_speed_above_limit()
{
    const WheelSpeedChecker checker(50.0);
    const WheelSpeedSample sample{100, 70.0};
    CHECK(checker.is_over_limit(sample), "70 km/h should be over a 50 km/h limit");
}

// Input: negative, NaN, and infinite limits. Expected: std::invalid_argument.
void test_invalid_limits_rejected()
{
    CHECK(constructor_rejects(-1.0), "limit -1 should be rejected");
    CHECK(constructor_rejects(nan("")), "limit NaN should be rejected");
    CHECK(constructor_rejects(numeric_limits<double>::infinity()),
          "limit +infinity should be rejected");
}

// Input: limit 0. Expected: accepted (zero is nonnegative), no exception.
void test_zero_limit_accepted()
{
    CHECK(!constructor_rejects(0.0), "limit 0 should be accepted");
}


// Input: limit 35.0, sample speed 35.0. Expected: is not over the limit, 
// as the is_over_limit function checks for strictly greater than the limit.

void test_speed_at_limit()
{
    const WheelSpeedChecker checker(35.0);
    const WheelSpeedSample sample{100, 35.0};
    CHECK(!checker.is_over_limit(sample), "35 km/h should not be over a 35 km/h limit");
}

// ---------------------------------------------------------------------
// YOUR TEST: speed exactly at the limit.
// Write the input and expected result in a comment first, then fill in
// the body. Remember to call your function from main() below.
//
// void test_speed_at_limit()
// {
// }
// ---------------------------------------------------------------------

} // namespace

int main()
{
    test_speed_below_limit();
    test_speed_above_limit();
    test_invalid_limits_rejected();
    test_zero_limit_accepted();
    test_speed_at_limit();

    // YOUR TEST: call test_speed_at_limit() here.

    if (g_failures != 0) {
        cerr << g_failures << " check(s) failed\n";
        return 1;
    }

    cout << "All wheel-speed checker tests passed\n";
    return 0;
}
