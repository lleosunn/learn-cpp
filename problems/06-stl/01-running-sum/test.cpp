#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    CHECK_EQ("running_sum({1,2,3,4})", running_sum({1, 2, 3, 4}), (std::vector<int>{1, 3, 6, 10}));
    CHECK_EQ("running_sum({})", running_sum({}), (std::vector<int>{}));
    CHECK_EQ("running_sum({-1,1})", running_sum({-1, 1}), (std::vector<int>{-1, 0}));
    CHECK_EQ("running_sum({5})", running_sum({5}), (std::vector<int>{5}));

    std::vector<int> big(1000, 1);
    std::vector<int> big_sum = running_sum(big);
    CHECK_EQ("running_sum of 1000 ones: size", big_sum.size(), std::size_t{1000});
    CHECK_EQ("running_sum of 1000 ones: last", big_sum.empty() ? -1 : big_sum.back(), 1000);

    std::vector<int> v{1, 2, 3, 4};
    int out = -1;
    CHECK_TRUE("get_at(v, 2) returns true", get_at(v, 2, out));
    CHECK_EQ("get_at(v, 2) writes 3", out, 3);
    CHECK_TRUE("get_at(v, 0) returns true", get_at(v, 0, out));
    CHECK_EQ("get_at(v, 0) writes 1", out, 1);

    out = -1;
    CHECK_TRUE("get_at(v, v.size()) returns false", !get_at(v, v.size(), out));
    CHECK_EQ("get_at(v, v.size()) leaves out untouched", out, -1);

    std::vector<int> empty;
    CHECK_TRUE("get_at({}, 0) returns false", !get_at(empty, 0, out));
}
