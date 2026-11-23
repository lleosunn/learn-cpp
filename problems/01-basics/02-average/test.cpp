#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    CHECK_NEAR("average(1, 2, 3)", average(1, 2, 3), 2.0, 1e-9);
    CHECK_NEAR("average(1, 2, 2)", average(1, 2, 2), 5.0 / 3.0, 1e-9);
    CHECK_NEAR("average(0, 0, 1)", average(0, 0, 1), 1.0 / 3.0, 1e-9);
    CHECK_NEAR("average(-1, -2, -3)", average(-1, -2, -3), -2.0, 1e-9);
    CHECK_NEAR("average(-1, 0, 0)", average(-1, 0, 0), -1.0 / 3.0, 1e-9);
    CHECK_NEAR("average(7, 7, 7)", average(7, 7, 7), 7.0, 1e-9);
    CHECK_NEAR("average(10, 20, 31)", average(10, 20, 31), 61.0 / 3.0, 1e-9);
}
