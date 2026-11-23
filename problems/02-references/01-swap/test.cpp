#include "check.h"
#include "tracked.h"
#include "solution.cpp"

#include <utility>

void run_tests() {
    {
        int a = 1, b = 2;
        swap_ints(a, b);
        CHECK_EQ("swap_ints(1, 2) -> (a, b)", (std::make_pair(a, b)), (std::make_pair(2, 1)));
    }
    {
        int a = 5, b = 5;
        swap_ints(a, b);
        CHECK_EQ("swap_ints(5, 5) -> (a, b)", (std::make_pair(a, b)), (std::make_pair(5, 5)));
    }
    {
        int a = -3, b = 7;
        swap_ints(a, b);
        CHECK_EQ("swap_ints(-3, 7) -> (a, b)", (std::make_pair(a, b)), (std::make_pair(7, -3)));
    }
    {
        int x = 9;
        swap_ints(x, x);
        CHECK_EQ("swap_ints(x, x) leaves x = 9", x, 9);
    }
    {
        int a = 2147483647, b = -2147483647 - 1;
        swap_ints(a, b);
        CHECK_EQ("swap_ints(INT_MAX, INT_MIN) -> (a, b)", (std::make_pair(a, b)), (std::make_pair(-2147483647 - 1, 2147483647)));
    }
    {
        int a = 1, b = 2;
        swap_ints(a, b);
        swap_ints(a, b);
        CHECK_EQ("swapping twice restores (1, 2)", (std::make_pair(a, b)), (std::make_pair(1, 2)));
    }
}
