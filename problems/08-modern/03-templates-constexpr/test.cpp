#include "check.h"
#include "tracked.h"
#include "solution.cpp"

bool is_even(int x) { return x % 2 == 0; }

void run_tests() {
    std::vector<int> ints{3, 9, 2, 7};
    std::vector<double> doubles{1.5, -2.0, 0.25};
    std::vector<std::string> strings{"pear", "apple", "fig"};
    std::vector<int> one{5};

    CHECK_EQ("max_of ints", max_of(ints), 9);
    CHECK_NEAR("max_of doubles", max_of(doubles), 1.5, 1e-12);
    CHECK_EQ("max_of strings", max_of(strings), std::string("pear"));
    CHECK_EQ("max_of single element", max_of(one), 5);

    std::vector<int> mm{3, 1, 2};
    CHECK_EQ("min_max ints", min_max(mm), (std::pair<int, int>{1, 3}));
    CHECK_EQ("min_max single element", min_max(one), (std::pair<int, int>{5, 5}));
    CHECK_EQ("min_max strings", min_max(strings), (std::pair<std::string, std::string>{"apple", "pear"}));

    std::vector<int> nums{1, 2, 3, 4, 5, 6};
    auto longer_than_3 = [](const std::string& s) { return s.size() > 3; };
    CHECK_EQ("filter with a plain function", filter(nums, is_even), (std::vector<int>{2, 4, 6}));
    CHECK_EQ("filter strings with a lambda", filter(strings, longer_than_3), (std::vector<std::string>{"pear", "apple"}));
    CHECK_EQ("filter keeps nothing", filter(nums, [](int x) { return x > 100; }), (std::vector<int>{}));
    CHECK_EQ("filter leaves input alone", nums, (std::vector<int>{1, 2, 3, 4, 5, 6}));

    CHECK_EQ("ipow(2, 10)", ipow(2, 10), 1024LL);
    CHECK_EQ("ipow(7, 0)", ipow(7, 0), 1LL);
    CHECK_EQ("ipow(-3, 3)", ipow(-3, 3), -27LL);
    CHECK_EQ("ipow(10, 18)", ipow(10, 18), 1000000000000000000LL);
    static_assert(ipow(3, 4) == 81, "ipow must be constexpr so the compiler can evaluate it");
    CHECK_TRUE("ipow(3, 4) evaluated at compile time", true);
}
