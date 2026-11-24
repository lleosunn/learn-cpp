#include "check.h"
#include "tracked.h"
#include "solution.cpp"

static std::vector<int> vec(const int* a, std::size_t n) { return std::vector<int>(a, a + n); }

void run_tests() {
    int a[5] = {1, 2, 3, 4, 5};
    reverse_array(a, 5);
    CHECK_EQ("reverse_array(a, 5) with a = {1, 2, 3, 4, 5}", vec(a, 5), (std::vector<int>{5, 4, 3, 2, 1}));
    reverse_array(a, 5);
    CHECK_EQ("reversing a again restores {1, 2, 3, 4, 5}", vec(a, 5), (std::vector<int>{1, 2, 3, 4, 5}));

    int b[4] = {1, 2, 3, 4};
    reverse_array(b, 4);
    CHECK_EQ("reverse_array(b, 4) with b = {1, 2, 3, 4} (even length)", vec(b, 4), (std::vector<int>{4, 3, 2, 1}));

    int two[2] = {8, 9};
    reverse_array(two, 2);
    CHECK_EQ("reverse_array(two, 2) with two = {8, 9}", vec(two, 2), (std::vector<int>{9, 8}));

    int one[1] = {7};
    reverse_array(one, 1);
    CHECK_EQ("reverse_array(one, 1) with one = {7}", vec(one, 1), (std::vector<int>{7}));

    int c[5] = {1, 2, 3, 4, 5};
    reverse_array(c, 3);
    CHECK_EQ("reverse_array(c, 3) with c = {1, 2, 3, 4, 5} (only the first 3)", vec(c, 5), (std::vector<int>{3, 2, 1, 4, 5}));

    int d[5] = {1, 2, 3, 4, 5};
    reverse_array(d + 1, 3);
    CHECK_EQ("reverse_array(d + 1, 3) with d = {1, 2, 3, 4, 5} (a slice from the middle)", vec(d, 5), (std::vector<int>{1, 4, 3, 2, 5}));

    int z[2] = {5, 6};
    reverse_array(z, 0);
    CHECK_EQ("reverse_array(z, 0) with z = {5, 6} changes nothing", vec(z, 2), (std::vector<int>{5, 6}));
    reverse_array(nullptr, 0);
    CHECK_TRUE("reverse_array(nullptr, 0) returns without crashing", true);
}
