#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "wheel_speed_checker.hpp"
#include "wheel_speed_csv.hpp"

using namespace std;

// argc is the number of command-line words, including the program name.
// argv holds those words: argv[0] is the program name, argv[1] the first argument.
int main(int argc, char* argv[])
{
    // Exactly one argument (the CSV path) is required. Checking argc first
    // makes sure argv[1] exists before we read it.
    if (argc != 2) {
        cerr << "Usage: wheel_speed_check <csv-file>\n";
        return 1;
    }

    const string csv_path = argv[1];

    // Any exception thrown inside this block (for example, a missing file
    // or an invalid CSV row) jumps to the catch block below.
    try {
        const vector<WheelSpeedSample> samples = read_wheel_speed_csv(csv_path);

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
