#include "check.h"
#include "tracked.h"
#include "solution.cpp"

#include <type_traits>

void run_tests() {
    CHECK_EQ("clamp_value(150)", clamp_value(150), 100);
    CHECK_EQ("clamp_value(-5)", clamp_value(-5), 0);
    CHECK_EQ("clamp_value(50)", clamp_value(50), 50);
    CHECK_EQ("clamp_value(7, 1, 5)", clamp_value(7, 1, 5), 5);
    CHECK_EQ("clamp_value(-10, -3)", clamp_value(-10, -3), -3);
    CHECK_NEAR("clamp_value(2.5)", clamp_value(2.5), 1.0, 1e-9);
    CHECK_NEAR("clamp_value(-0.5)", clamp_value(-0.5), 0.0, 1e-9);
    CHECK_NEAR("clamp_value(0.5)", clamp_value(0.5), 0.5, 1e-9);
    CHECK_NEAR("clamp_value(0.25, 0.5, 0.75)", clamp_value(0.25, 0.5, 0.75), 0.5, 1e-9);
    CHECK_TRUE("clamp_value(150) resolves to the int overload", (std::is_same_v<decltype(clamp_value(150)), int>));
    CHECK_TRUE("clamp_value(2.5) resolves to the double overload", (std::is_same_v<decltype(clamp_value(2.5)), double>));
}
