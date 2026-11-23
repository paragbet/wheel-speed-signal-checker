#include <exception>
#include <iostream>
#include <vector>

#include "wheel_speed_checker.hpp"
#include "wheel_speed_csv.hpp"

using namespace std;

int main()
{
    // Any exception thrown inside this block (for example, a missing file
    // or an invalid CSV row) jumps to the catch block below.
    try {
        // The path is relative, so run the program from the repository root.
        const vector<WheelSpeedSample> samples = read_wheel_speed_csv("samples/normal.csv");

        const WheelSpeedChecker checker(35.0);
        int samples_over_limit = 0;

        // Visit each sample in file order. "const &" reads the existing
        // sample without copying or changing it.
        for (const WheelSpeedSample& sample : samples) {
            if (checker.is_over_limit(sample)) {
                cout << "Over limit at " << sample.timestamp_ms << " ms: "
                     << sample.wheel_speed_kmh << " km/h\n";
                ++samples_over_limit;
            }
        }

        cout << "Samples checked: " << samples.size() << "\n";
        cout << "Samples over limit: " << samples_over_limit << "\n";
    } catch (const exception& error) {
        cerr << "Error: " << error.what() << "\n";
        return 1;
    }

    return 0;
}
