#include "check.h"
#include "tracked.h"
#include "solution.cpp"

static std::vector<int> removed(std::vector<int> v, int x) {
    remove_all(v, x);
    return v;
}

void run_tests() {
    CHECK_EQ("remove_all({1,2,1,3}, 1)", removed({1, 2, 1, 3}, 1), (std::vector<int>{2, 3}));
    CHECK_EQ("remove_all({1,1,2}, 1) handles adjacent duplicates", removed({1, 1, 2}, 1), (std::vector<int>{2}));
    CHECK_EQ("remove_all({7,7,7}, 7) removes everything", removed({7, 7, 7}, 7), (std::vector<int>{}));
    CHECK_EQ("remove_all({1,2,3}, 9) removes nothing", removed({1, 2, 3}, 9), (std::vector<int>{1, 2, 3}));
    CHECK_EQ("remove_all({}, 1)", removed({}, 1), (std::vector<int>{}));
    CHECK_EQ("remove_all keeps order of the rest", removed({5, 0, 4, 0, 3, 0, 0, 2}, 0), (std::vector<int>{5, 4, 3, 2}));

    CHECK_EQ("dedupe_keep_order({3,1,3,2,1})", dedupe_keep_order({3, 1, 3, 2, 1}), (std::vector<int>{3, 1, 2}));
    CHECK_EQ("dedupe_keep_order({})", dedupe_keep_order({}), (std::vector<int>{}));
    CHECK_EQ("dedupe_keep_order({5,5,5})", dedupe_keep_order({5, 5, 5}), (std::vector<int>{5}));
    CHECK_EQ("dedupe_keep_order already unique", dedupe_keep_order({9, 8, 7}), (std::vector<int>{9, 8, 7}));

    CHECK_EQ("sorted_intersection({1,1,2,5}, {1,3,5,5})", sorted_intersection({1, 1, 2, 5}, {1, 3, 5, 5}), (std::vector<int>{1, 5}));
    CHECK_EQ("sorted_intersection disjoint", sorted_intersection({1, 2}, {3, 4}), (std::vector<int>{}));
    CHECK_EQ("sorted_intersection unsorted inputs", sorted_intersection({5, 3, 1}, {1, 5, 9}), (std::vector<int>{1, 5}));
    CHECK_EQ("sorted_intersection with empty", sorted_intersection({}, {1, 2}), (std::vector<int>{}));
}
