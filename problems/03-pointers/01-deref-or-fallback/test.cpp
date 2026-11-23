#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    int x = 5;
    CHECK_EQ("value_or(&x, -1) with x = 5", value_or(&x, -1), 5);
    int y = 9;
    CHECK_EQ("value_or(&y, 0) with y = 9", value_or(&y, 0), 9);
    int w = -3;
    CHECK_EQ("value_or(&w, 7) with w = -3", value_or(&w, 7), -3);
    int z = 0;
    CHECK_EQ("value_or(&z, -1) with z = 0 (pointee is zero, pointer is not null)", value_or(&z, -1), 0);
    CHECK_EQ("value_or(nullptr, -1)", value_or(nullptr, -1), -1);
    CHECK_EQ("value_or(nullptr, 42)", value_or(nullptr, 42), 42);
}
