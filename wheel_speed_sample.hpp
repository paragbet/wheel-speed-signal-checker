#pragma once

#include <cstdint>

struct WheelSpeedSample {
    std::int64_t timestamp_ms; // Timestamp in milliseconds
    double wheel_speed_kmh;    // Wheel speed in kilometers per hour
};