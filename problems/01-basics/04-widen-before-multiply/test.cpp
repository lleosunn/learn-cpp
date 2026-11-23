#include "check.h"
#include "tracked.h"
#include "solution.cpp"

#include <climits>  // INT_MAX, INT_MIN

void run_tests() {
    CHECK_EQ("product(3, 4)", product(3, 4), 12LL);
    CHECK_EQ("product(100000, 100000)", product(100000, 100000), 10000000000LL);
    CHECK_EQ("product(-100000, 100000)", product(-100000, 100000), -10000000000LL);
    CHECK_EQ("product(INT_MAX, 1)", product(INT_MAX, 1), 2147483647LL);
    CHECK_EQ("product(INT_MIN, -1)", product(INT_MIN, -1), 2147483648LL);
    CHECK_EQ("product(INT_MIN, INT_MIN)", product(INT_MIN, INT_MIN), 4611686018427387904LL);
    CHECK_EQ("product(0, INT_MAX)", product(0, INT_MAX), 0LL);
}
