#include "speed_limit.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>

using namespace std;

double parse_speed_limit(const string& text)
{
    // istringstream lets us read from a string the same way cin reads
    // from the keyboard.
    istringstream input(text);

    // 1. Read a number. ">>" skips leading spaces first. It fails if the
    //    text does not start with a number (for example "", "fast").
    double limit_kmh = 0.0;
    if (!(input >> limit_kmh)) {
        throw invalid_argument("speed limit '" + text + "' is not a number");
    }

    // 2. Try to read one more word. Trailing spaces are skipped, so this
    //    only succeeds if real text is left over (for example "40extra").
    string leftover;
    if (input >> leftover) {
        throw invalid_argument("speed limit '" + text +
                               "' has extra text after the number");
    }

    // 3. Check the value itself.
    if (!isfinite(limit_kmh)) {
        throw invalid_argument("speed limit '" + text + "' must be a finite number");
    }
    if (limit_kmh < 0.0) {
        throw invalid_argument("speed limit '" + text + "' must not be negative");
    }

    return limit_kmh;
}
