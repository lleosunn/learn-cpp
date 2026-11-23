#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    int x = 1, y = 2;
    CHECK_EQ("swap_through(&x, &y) with x = 1, y = 2 returns true", swap_through(&x, &y), true);
    CHECK_EQ("x == 2 after swap_through(&x, &y)", x, 2);
    CHECK_EQ("y == 1 after swap_through(&x, &y)", y, 1);

    int s = 3;
    CHECK_EQ("swap_through(&s, &s) returns true", swap_through(&s, &s), true);
    CHECK_EQ("s unchanged after swap_through(&s, &s)", s, 3);

    int q = 11;
    CHECK_EQ("swap_through(nullptr, &q) returns false", swap_through(nullptr, &q), false);
    CHECK_EQ("q unchanged after swap_through(nullptr, &q)", q, 11);
    CHECK_EQ("swap_through(&q, nullptr) returns false", swap_through(&q, nullptr), false);
    CHECK_EQ("q unchanged after swap_through(&q, nullptr)", q, 11);
    CHECK_EQ("swap_through(nullptr, nullptr) returns false", swap_through(nullptr, nullptr), false);
}
