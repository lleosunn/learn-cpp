#include "check.h"
#include "tracked.h"
#include "solution.cpp"

#include <type_traits>

static int use_and_return_early(int v) {
    ScopedTracked s(v);
    if (v > 0) return s.get().value * 2;  // leaves the scope early
    return 0;
}

void run_tests() {
    {
        Tracked::reset();
        {
            ScopedTracked s(7);
            CHECK_EQ("alive == 1 inside scope", Tracked::alive, 1);
            CHECK_EQ("get().value == 7", s.get().value, 7);
        }
        CHECK_EQ("alive == 0 after scope", Tracked::alive, 0);
        CHECK_EQ("destroyed == 1 after scope", Tracked::destroyed, 1);
    }
    {
        Tracked::reset();
        CHECK_EQ("early return still computes the value", use_and_return_early(21), 42);
        CHECK_EQ("alive == 0 after early return", Tracked::alive, 0);
    }
    {
        Tracked::reset();
        Tracked* raw = nullptr;
        {
            ScopedTracked s(3);
            raw = s.release();
            CHECK_TRUE("release() returns the owned object", raw != nullptr && raw->value == 3);
        }
        CHECK_EQ("alive == 1 after scope when released", Tracked::alive, 1);
        delete raw;  // the test owns it now; a destructor that ignores release() double-frees here
        CHECK_EQ("alive == 0 after deleting the released pointer", Tracked::alive, 0);
    }
    {
        CHECK_TRUE("ScopedTracked is not copy constructible", !std::is_copy_constructible_v<ScopedTracked>);
        CHECK_TRUE("ScopedTracked is not copy assignable", !std::is_copy_assignable_v<ScopedTracked>);
    }
    {
        Tracked::reset();
        {
            ScopedTracked a(1);
            ScopedTracked b(2);
            CHECK_EQ("alive == 2 with two wrappers", Tracked::alive, 2);
            CHECK_EQ("each wrapper has its own object", a.get().value + b.get().value, 3);
        }
        CHECK_EQ("alive == 0 after both wrappers die", Tracked::alive, 0);
    }
    {
        Tracked::reset();
        {
            const ScopedTracked s(9);
            CHECK_EQ("get() works on a const wrapper", s.get().value, 9);
        }
        CHECK_EQ("alive == 0 after a const wrapper dies", Tracked::alive, 0);
    }
    {
        Tracked::reset();
        for (int i = 0; i < 100; ++i) {
            ScopedTracked s(i);
            s.get().value += 1;
        }
        CHECK_EQ("alive == 0 after 100 loop iterations", Tracked::alive, 0);
        CHECK_EQ("destroyed == 100 after 100 loop iterations", Tracked::destroyed, 100);
    }
}
