#include "check.h"
#include "tracked.h"
#include "solution.cpp"

#include <vector>

static Tracked* make_block(const std::vector<int>& vals) {
    Tracked* p = new Tracked[vals.size()];
    for (std::size_t i = 0; i < vals.size(); ++i) p[i].value = vals[i];
    return p;
}

static std::vector<int> values_of(const Tracked* p, std::size_t n) {
    std::vector<int> out;
    if (p) for (std::size_t i = 0; i < n; ++i) out.push_back(p[i].value);
    return out;
}

void run_tests() {
    {
        Tracked::reset();
        Tracked* a = make_block({1, 2, 3});
        Tracked* b = grow(a, 3, 5);
        CHECK_EQ("grow {1,2,3} to 5 keeps 1,2,3 and zero-fills", values_of(b, 5), (std::vector<int>{1, 2, 3, 0, 0}));
        CHECK_EQ("alive == 5 right after grow (old block released)", Tracked::alive, 5);
        delete[] b;
        CHECK_EQ("alive == 0 after freeing the result", Tracked::alive, 0);
    }
    {
        Tracked::reset();
        Tracked* a = make_block({1, 2, 3});
        Tracked* b = grow(a, 3, 2);
        CHECK_EQ("shrink {1,2,3} to 2 keeps 1,2", values_of(b, 2), (std::vector<int>{1, 2}));
        CHECK_EQ("alive == 2 after shrinking", Tracked::alive, 2);
        delete[] b;
        CHECK_EQ("alive == 0 after freeing the shrunk result", Tracked::alive, 0);
    }
    {
        Tracked::reset();
        Tracked* b = grow(nullptr, 0, 4);
        CHECK_EQ("grow from nullptr gives 4 zeros", values_of(b, 4), (std::vector<int>{0, 0, 0, 0}));
        CHECK_EQ("alive == 4 after growing from nullptr", Tracked::alive, 4);
        delete[] b;
        CHECK_EQ("alive == 0 after freeing it", Tracked::alive, 0);
    }
    {
        Tracked::reset();
        Tracked* a = make_block({8, 9});
        Tracked* b = grow(a, 2, 0);
        CHECK_EQ("alive == 0 after growing to zero elements", Tracked::alive, 0);
        delete[] b;
        CHECK_EQ("alive still 0 after freeing the empty result", Tracked::alive, 0);
    }
    {
        // What IntStack (problem 5) will do: double the block each time it fills.
        Tracked::reset();
        Tracked* p = nullptr;
        std::size_t cap = 0, n = 0;
        for (int v = 1; v <= 8; ++v) {
            if (n == cap) {
                std::size_t new_cap = cap ? cap * 2 : 1;
                p = grow(p, cap, new_cap);
                cap = new_cap;
            }
            if (p) p[n].value = v;
            ++n;
        }
        CHECK_EQ("doubling 0->1->2->4->8 preserves every value", values_of(p, 8), (std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8}));
        CHECK_EQ("alive == 8 after four grows (no stale blocks)", Tracked::alive, 8);
        delete[] p;
        CHECK_EQ("alive == 0 after freeing the final block", Tracked::alive, 0);
    }
}
