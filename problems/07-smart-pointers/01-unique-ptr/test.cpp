#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    Tracked::reset();
    {
        auto p = make_tracked(5);
        CHECK_TRUE("make_tracked(5) returns a non-empty pointer", p != nullptr);
        CHECK_EQ("one Tracked alive while p lives", Tracked::alive, 1);
        CHECK_EQ("value_of(p) == 5", value_of(p), 5);
        CHECK_TRUE("value_of does not empty p", p != nullptr);
    }
    CHECK_EQ("destroyed when p leaves scope", Tracked::alive, 0);

    CHECK_EQ("value_of(empty) == -1", value_of(std::unique_ptr<Tracked>{}), -1);

    Tracked::reset();
    {
        auto p = make_tracked(1);
        replace_with(p, 9);
        CHECK_EQ("replace_with(p, 9): p->value", p ? p->value : -1, 9);
        CHECK_EQ("replace_with: old object destroyed", Tracked::destroyed, 1);
        CHECK_EQ("replace_with: exactly one alive", Tracked::alive, 1);
    }
    CHECK_EQ("alive == 0 after replace_with scope", Tracked::alive, 0);

    Tracked::reset();
    {
        std::unique_ptr<Tracked> q;            // starts empty
        replace_with(q, 4);
        CHECK_EQ("replace_with on an empty pointer", value_of(q), 4);
        CHECK_EQ("nothing destroyed when there was no old object", Tracked::destroyed, 0);
    }

    Tracked::reset();
    {
        auto p = make_tracked(1);
        replace_with(p, 2);
        value_of(p);
        CHECK_EQ("no Tracked copies anywhere", Tracked::copies, 0);
        CHECK_EQ("no Tracked moves anywhere (only the pointer moves)", Tracked::moves, 0);
        CHECK_EQ("exactly two Tracked ever constructed", Tracked::constructed, 2);
    }
    CHECK_EQ("alive == 0 at the end", Tracked::alive, 0);
}
