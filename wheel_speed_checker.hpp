#pragma once

#include "wheel_speed_sample.hpp"

// Checks wheel-speed samples against a fixed speed limit.
class WheelSpeedChecker {
public:
    // Stores the speed limit in km/h. Throws std::invalid_argument if the
    // limit is not finite (NaN or infinity) or is negative.
    explicit WheelSpeedChecker(double speed_limit_kmh);

    // Returns true if the sample's speed is strictly greater than the limit.
    // The sample is expected to be already validated by the CSV reader.
    bool is_over_limit(const WheelSpeedSample& sample) const;

private:
    double limit_kmh;
};
