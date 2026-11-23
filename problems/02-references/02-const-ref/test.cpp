#include "check.h"
#include "tracked.h"
#include "solution.cpp"

#include <vector>

void run_tests() {
    {
        std::vector<Tracked> v = {1, 2, 3};
        Tracked::reset();
        CHECK_EQ("sum_values({1, 2, 3})", sum_values(v), 6);
        CHECK_EQ("sum_values({1, 2, 3}) makes no copies", Tracked::copies, 0);
    }
    {
        std::vector<Tracked> v;
        Tracked::reset();
        CHECK_EQ("sum_values({})", sum_values(v), 0);
        CHECK_EQ("sum_values({}) makes no copies", Tracked::copies, 0);
    }
    {
        std::vector<Tracked> v = {-5, 5};
        Tracked::reset();
        CHECK_EQ("sum_values({-5, 5})", sum_values(v), 0);
        CHECK_EQ("sum_values({-5, 5}) makes no copies", Tracked::copies, 0);
    }
    {
        std::vector<Tracked> v;
        v.reserve(1000);
        for (int i = 1; i <= 1000; ++i) v.emplace_back(i);
        Tracked::reset();
        CHECK_EQ("sum_values(1..1000)", sum_values(v), 500500);
        CHECK_EQ("sum_values(1..1000) makes no copies", Tracked::copies, 0);
        CHECK_EQ("sum_values(1..1000) constructs no Tracked at all", Tracked::constructed, 0);
    }
}
