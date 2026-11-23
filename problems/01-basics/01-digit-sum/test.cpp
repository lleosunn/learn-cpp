#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    CHECK_EQ("digit_sum(0)", digit_sum(0), 0);
    CHECK_EQ("digit_sum(7)", digit_sum(7), 7);
    CHECK_EQ("digit_sum(1234)", digit_sum(1234), 10);
    CHECK_EQ("digit_sum(-99)", digit_sum(-99), 18);
    CHECK_EQ("digit_sum(1000000)", digit_sum(1000000), 1);
}
