#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    int a[4] = {4, 3, 3, 1};
    int* out = nullptr;
    CHECK_EQ("find_first(a, 4, 3, &out) with a = {4, 3, 3, 1} returns true", find_first(a, 4, 3, &out), true);
    CHECK_TRUE("out == &a[1] (the first 3, not the second)", out == &a[1]);
    if (out) *out = 99;
    CHECK_EQ("a[1] == 99 after writing *out = 99 (the pointer is live)", a[1], 99);

    int* stale = &a[0];
    CHECK_EQ("find_first(a, 4, 7, &stale) returns false", find_first(a, 4, 7, &stale), false);
    CHECK_TRUE("stale == nullptr after a miss (reset, not left pointing at a[0])", stale == nullptr);

    int* lastp = nullptr;
    CHECK_EQ("find_first(a, 4, 1, &lastp) returns true", find_first(a, 4, 1, &lastp), true);
    CHECK_TRUE("lastp == &a[3] (match in the last slot)", lastp == &a[3]);

    CHECK_EQ("find_first(a, 4, 4, nullptr) returns true without crashing", find_first(a, 4, 4, nullptr), true);
    CHECK_EQ("find_first(a, 4, 7, nullptr) returns false without crashing", find_first(a, 4, 7, nullptr), false);

    int* none = &a[2];
    CHECK_EQ("find_first(nullptr, 0, 3, &none) returns false", find_first(nullptr, 0, 3, &none), false);
    CHECK_TRUE("none == nullptr after n == 0", none == nullptr);
}
