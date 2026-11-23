#pragma once

#include <string>

// Converts command-line text such as "40" or "35.5" into a speed limit in
// km/h. Leading and trailing whitespace is allowed. Throws
// std::invalid_argument if the text is not a single number, or if the
// number is not finite (NaN or infinity) or is negative.
double parse_speed_limit(const std::string& text);
