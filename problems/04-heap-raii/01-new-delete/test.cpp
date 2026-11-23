#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    {
        Tracked::reset();
        Tracked* p = make_tracked(7);
        CHECK_TRUE("make_tracked(7) returns a pointer", p != nullptr);
        CHECK_EQ("alive == 1 after make", Tracked::alive, 1);
        CHECK_EQ("constructed == 1 after make (no temporaries)", Tracked::constructed, 1);
        CHECK_EQ("p->value == 7", p ? p->value : -1, 7);
        destroy_tracked(p);
        CHECK_EQ("alive == 0 after destroy", Tracked::alive, 0);
        CHECK_EQ("destroyed == 1 after destroy", Tracked::destroyed, 1);
    }
    {
        Tracked::reset();
        destroy_tracked(nullptr);
        CHECK_EQ("destroy_tracked(nullptr) destroys nothing", Tracked::destroyed, 0);
    }
    {
        Tracked::reset();
        Tracked* a = make_tracked(1);
        Tracked* b = make_tracked(2);
        CHECK_EQ("alive == 2 after two makes", Tracked::alive, 2);
        CHECK_TRUE("two makes give two distinct objects", a != b);
        CHECK_EQ("second object keeps its own value", b ? b->value : -1, 2);
        destroy_tracked(b);
        CHECK_EQ("alive == 1 after destroying the second", Tracked::alive, 1);
        CHECK_EQ("first object still readable", a ? a->value : -1, 1);
        destroy_tracked(a);
        CHECK_EQ("alive == 0 after destroying both", Tracked::alive, 0);
    }
    {
        Tracked::reset();
        Tracked* ps[10];
        for (int i = 0; i < 10; ++i) ps[i] = make_tracked(i * i);
        CHECK_EQ("alive == 10 after ten makes", Tracked::alive, 10);
        int sum = 0;
        for (int i = 0; i < 10; ++i) sum += ps[i] ? ps[i]->value : 0;
        CHECK_EQ("values survive across calls (sum of squares 0..9)", sum, 285);
        for (int i = 0; i < 10; ++i) destroy_tracked(ps[i]);
        CHECK_EQ("alive == 0 after destroying all ten", Tracked::alive, 0);
    }
}
