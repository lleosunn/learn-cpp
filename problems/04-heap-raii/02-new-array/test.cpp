#include "check.h"
#include "tracked.h"
#include "solution.cpp"

#include <vector>

static std::vector<int> values_of(const Tracked* p, std::size_t n) {
    std::vector<int> out;
    if (p) for (std::size_t i = 0; i < n; ++i) out.push_back(p[i].value);
    return out;
}

void run_tests() {
    {
        Tracked::reset();
        Tracked* p = make_tracked_array(3, 10);
        CHECK_TRUE("make_tracked_array(3, 10) returns a pointer", p != nullptr);
        CHECK_EQ("alive == 3 after make", Tracked::alive, 3);
        CHECK_EQ("values are 10, 11, 12", values_of(p, 3), (std::vector<int>{10, 11, 12}));
        free_tracked_array(p);
        CHECK_EQ("alive == 0 after free", Tracked::alive, 0);
        CHECK_EQ("destroyed == 3 after free", Tracked::destroyed, 3);
    }
    {
        Tracked::reset();
        Tracked* p = make_tracked_array(0, 99);
        free_tracked_array(p);
        CHECK_EQ("alive == 0 after making and freeing an empty array", Tracked::alive, 0);
    }
    {
        Tracked::reset();
        Tracked* p = make_tracked_array(1, -4);
        CHECK_EQ("single element has value -4", values_of(p, 1), (std::vector<int>{-4}));
        CHECK_EQ("alive == 1 for a one-element array", Tracked::alive, 1);
        free_tracked_array(p);
        CHECK_EQ("alive == 0 after freeing one element", Tracked::alive, 0);
    }
    {
        Tracked::reset();
        Tracked* p = make_tracked_array(1000, 5);
        CHECK_EQ("alive == 1000 after a large make", Tracked::alive, 1000);
        CHECK_EQ("p[0].value == 5", p ? p[0].value : -1, 5);
        CHECK_EQ("p[999].value == 1004", p ? p[999].value : -1, 1004);
        free_tracked_array(p);
        CHECK_EQ("alive == 0 after freeing 1000 elements", Tracked::alive, 0);
        CHECK_EQ("destroyed == 1000 after freeing 1000 elements", Tracked::destroyed, 1000);
    }
    {
        Tracked::reset();
        Tracked* a = make_tracked_array(2, 0);
        Tracked* b = make_tracked_array(2, 100);
        CHECK_EQ("alive == 4 with two arrays", Tracked::alive, 4);
        CHECK_EQ("first array has its own values", values_of(a, 2), (std::vector<int>{0, 1}));
        free_tracked_array(a);
        CHECK_EQ("alive == 2 after freeing the first array", Tracked::alive, 2);
        CHECK_EQ("second array intact after freeing the first", values_of(b, 2), (std::vector<int>{100, 101}));
        free_tracked_array(b);
        CHECK_EQ("alive == 0 after freeing both", Tracked::alive, 0);
    }
}
