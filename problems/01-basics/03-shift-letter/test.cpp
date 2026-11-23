#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    CHECK_EQ("shift_letter('a', 1)", shift_letter('a', 1), 'b');
    CHECK_EQ("shift_letter('z', 1)", shift_letter('z', 1), 'a');
    CHECK_EQ("shift_letter('y', 3)", shift_letter('y', 3), 'b');
    CHECK_EQ("shift_letter('m', 26)", shift_letter('m', 26), 'm');
    CHECK_EQ("shift_letter('a', 1000)", shift_letter('a', 1000), 'm');
    CHECK_EQ("shift_letter('q', 0)", shift_letter('q', 0), 'q');
    CHECK_EQ("shift_letter('A', 1)", shift_letter('A', 1), 'A');
    CHECK_EQ("shift_letter('!', 5)", shift_letter('!', 5), '!');
}
