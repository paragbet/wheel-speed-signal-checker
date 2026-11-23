// Dependency-free tests for parse_speed_limit.
//
// Usage: speed_limit_tests

#include <iostream>
#include <stdexcept>
#include <string>

#include "speed_limit.hpp"

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

// Returns true if parsing this text throws std::invalid_argument.
bool parse_rejects(const string& text)
{
    try {
        parse_speed_limit(text);
    } catch (const invalid_argument&) {
        return true;
    }
    return false;
}

// Input: "40". Expected: 40.0.
void test_whole_number_limit()
{
    CHECK(parse_speed_limit("40") == 40.0, "\"40\" should parse as 40.0");
}

// Input: "35.5". Expected: 35.5.
// Exact == is safe here: the text and the literal 35.5 convert to the
// same double.
void test_decimal_limit()
{
    CHECK(parse_speed_limit("35.5") == 35.5, "\"35.5\" should parse as 35.5");
}

// Input: "0". Expected: 0.0 (zero is not negative, so it is accepted).
void test_zero_limit()
{
    CHECK(parse_speed_limit("0") == 0.0, "\"0\" should parse as 0.0");
}

// Input: "  40  ". Expected: 40.0 (surrounding spaces are allowed).
void test_surrounding_whitespace_accepted()
{
    CHECK(parse_speed_limit("  40  ") == 40.0, "\"  40  \" should parse as 40.0");
}

// Input: "-5". Expected: std::invalid_argument.
void test_negative_limit_rejected()
{
    CHECK(parse_rejects("-5"), "\"-5\" should be rejected");
}

// Input: "fast". Expected: std::invalid_argument.
void test_nonnumeric_limit_rejected()
{
    CHECK(parse_rejects("fast"), "\"fast\" should be rejected");
}

// Input: "". Expected: std::invalid_argument.
void test_empty_limit_rejected()
{
    CHECK(parse_rejects(""), "empty text should be rejected");
}

// Input: "40extra". Expected: std::invalid_argument.
void test_number_with_extra_text_rejected()
{
    CHECK(parse_rejects("40extra"), "\"40extra\" should be rejected");
}

// ---------------------------------------------------------------------
// YOUR TEST: a number followed by extra text.
// Input: "40extra". Write the expected result in the comment first,
// then fill in the body (look at the tests above for a helper to reuse).
// Remember to call your function from main() below.
//
// void test_number_with_extra_text_rejected()
// {
// }
// ---------------------------------------------------------------------

} // namespace

int main()
{
    test_whole_number_limit();
    test_decimal_limit();
    test_zero_limit();
    test_surrounding_whitespace_accepted();
    test_negative_limit_rejected();
    test_nonnumeric_limit_rejected();
    test_empty_limit_rejected();
    test_number_with_extra_text_rejected();

    // YOUR TEST: call test_number_with_extra_text_rejected() here.

    if (g_failures != 0) {
        cerr << g_failures << " check(s) failed\n";
        return 1;
    }

    cout << "All speed limit tests passed\n";
    return 0;
}
