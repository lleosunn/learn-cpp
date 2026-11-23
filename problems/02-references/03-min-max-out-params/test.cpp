#include "check.h"
#include "tracked.h"
#include "solution.cpp"

#include <utility>
#include <vector>

void run_tests() {
    {
        std::vector<int> v = {3, 1, 2};
        int lo = 42, hi = 42;
        CHECK_TRUE("min_max({3, 1, 2}) returns true", min_max(v, lo, hi));
        CHECK_EQ("min_max({3, 1, 2}) -> (lo, hi)", (std::make_pair(lo, hi)), (std::make_pair(1, 3)));
    }
    {
        std::vector<int> v = {7};
        int lo = 42, hi = 42;
        CHECK_TRUE("min_max({7}) returns true", min_max(v, lo, hi));
        CHECK_EQ("min_max({7}) -> (lo, hi)", (std::make_pair(lo, hi)), (std::make_pair(7, 7)));
    }
    {
        std::vector<int> v = {-4, -9, 0};
        int lo = 42, hi = 42;
        CHECK_TRUE("min_max({-4, -9, 0}) returns true", min_max(v, lo, hi));
        CHECK_EQ("min_max({-4, -9, 0}) -> (lo, hi)", (std::make_pair(lo, hi)), (std::make_pair(-9, 0)));
    }
    {
        std::vector<int> v = {5, 5, 5};
        int lo = 42, hi = 42;
        CHECK_TRUE("min_max({5, 5, 5}) returns true", min_max(v, lo, hi));
        CHECK_EQ("min_max({5, 5, 5}) -> (lo, hi)", (std::make_pair(lo, hi)), (std::make_pair(5, 5)));
    }
    {
        std::vector<int> v = {9, 4, 1};
        int lo = 42, hi = 42;
        CHECK_TRUE("min_max({9, 4, 1}) returns true", min_max(v, lo, hi));
        CHECK_EQ("min_max({9, 4, 1}) -> (lo, hi)", (std::make_pair(lo, hi)), (std::make_pair(1, 9)));
    }
    {
        std::vector<int> v;
        int lo = 42, hi = 42;
        CHECK_TRUE("min_max({}) returns false", !min_max(v, lo, hi));
        CHECK_EQ("min_max({}) leaves (lo, hi) at the preset 42", (std::make_pair(lo, hi)), (std::make_pair(42, 42)));
    }
}
