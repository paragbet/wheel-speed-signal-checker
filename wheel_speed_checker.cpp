#include "wheel_speed_checker.hpp"

#include <cmath>
#include <stdexcept>

using namespace std;

namespace {

// Returns the limit unchanged if it is valid, otherwise throws.
// A helper lets the constructor validate the value inside its initializer list.
double validated_limit(double speed_limit_kmh)
{
    // isfinite is checked first: NaN compares false with everything,
    // so "NaN < 0.0" alone would not catch it.
    if (!isfinite(speed_limit_kmh)) {
        throw invalid_argument("speed limit must be a finite number");
    }
    if (speed_limit_kmh < 0.0) {
        throw invalid_argument("speed limit must not be negative");
    }
    return speed_limit_kmh;
}

} // namespace

WheelSpeedChecker::WheelSpeedChecker(double speed_limit_kmh)
    : limit_kmh(validated_limit(speed_limit_kmh))
{
}

bool WheelSpeedChecker::is_over_limit(const WheelSpeedSample& sample) const
{
    return sample.wheel_speed_kmh > limit_kmh;
}
