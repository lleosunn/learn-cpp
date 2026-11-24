#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    int a[6] = {4, 7, 1, 7, 9, 7};
    CHECK_TRUE("find_in_range(a, a + 6, 7) == a + 1 with a = {4, 7, 1, 7, 9, 7} (the first 7)", find_in_range(a, a + 6, 7) == a + 1);
    CHECK_TRUE("find_in_range(a, a + 6, 4) == a", find_in_range(a, a + 6, 4) == a);
    CHECK_TRUE("find_in_range(a, a + 6, 9) == a + 4", find_in_range(a, a + 6, 9) == a + 4);
    CHECK_TRUE("find_in_range(a, a + 6, 5) == a + 6 (a miss returns last)", find_in_range(a, a + 6, 5) == a + 6);
    CHECK_TRUE("find_in_range(a + 2, a + 6, 7) == a + 3 (the search starts after the first 7)", find_in_range(a + 2, a + 6, 7) == a + 3);
    CHECK_TRUE("find_in_range(a, a + 1, 7) == a + 1 (7 is outside [a, a + 1), so last)", find_in_range(a, a + 1, 7) == a + 1);
    CHECK_TRUE("find_in_range(a + 2, a + 2, 1) == a + 2 (empty range, even though a[2] == 1)", find_in_range(a + 2, a + 2, 1) == a + 2);
    CHECK_TRUE("find_in_range(nullptr, nullptr, 3) == nullptr (empty range; last happens to be null)", find_in_range(nullptr, nullptr, 3) == nullptr);

    CHECK_EQ("count_in_range(a, a + 6, 7)", count_in_range(a, a + 6, 7), std::size_t{3});
    CHECK_EQ("count_in_range(a + 2, a + 6, 7)", count_in_range(a + 2, a + 6, 7), std::size_t{2});
    CHECK_EQ("count_in_range(a + 5, a + 6, 7) (just the last element)", count_in_range(a + 5, a + 6, 7), std::size_t{1});
    CHECK_EQ("count_in_range(a, a + 6, 5)", count_in_range(a, a + 6, 5), std::size_t{0});
    CHECK_EQ("count_in_range(a, a, 4) (empty range)", count_in_range(a, a, 4), std::size_t{0});
}
