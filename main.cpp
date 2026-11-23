#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "wheel_speed_checker.hpp"
#include "speed_limit.hpp"
#include "wheel_speed_csv.hpp"

using namespace std;

// argc is the number of command-line words, including the program name.
// argv holds those words: argv[0] is the program name, argv[1] the CSV path,
// and argv[2] the optional speed limit.
int main(int argc, char* argv[])
{
    // The CSV path is required and the limit is optional, so argc must be
    // 2 or 3. Checking argc first makes sure argv[1] exists before we read it.
    if (argc != 2 && argc != 3) {
        cerr << "Usage: wheel_speed_check <csv-file> [limit-kmh]\n";
        return 1;
    }

    const string csv_path = argv[1];

    // Any exception thrown inside this block (for example, an invalid limit,
    // a missing file, or an invalid CSV row) jumps to the catch block below.
    try {
        // Use the default limit unless one was given. argv[2] exists only
        // when argc == 3.
        double speed_limit_kmh = 35.0;
        if (argc == 3) {
            speed_limit_kmh = parse_speed_limit(argv[2]);
        }

        const vector<WheelSpeedSample> samples = read_wheel_speed_csv(csv_path);

        // The checker validates the limit again, as a safety net.
        const WheelSpeedChecker checker(speed_limit_kmh);
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
