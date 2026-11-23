#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    Tracked::reset();
    {
        Tray tray;
        tray.put(std::make_unique<Tracked>(1));
        tray.put(std::make_unique<Tracked>(2));
        tray.put(std::make_unique<Tracked>(3));
        CHECK_EQ("size() after three puts", tray.size(), 3u);
        CHECK_EQ("alive after three puts (tray owns them)", Tracked::alive, 3);

        auto t = tray.take();
        CHECK_TRUE("take() returns a non-empty pointer", t != nullptr);
        CHECK_EQ("take() gives the most recent item", t ? t->value : -1, 3);
        CHECK_EQ("size() after take()", tray.size(), 2u);
        CHECK_EQ("alive after take() (t owns it now)", Tracked::alive, 3);

        t.reset();
        CHECK_EQ("alive after t.reset()", Tracked::alive, 2);
    }
    CHECK_EQ("alive after the tray is destroyed", Tracked::alive, 0);

    {
        Tray tray;
        CHECK_TRUE("take() on an empty tray is nullptr", tray.take() == nullptr);
        CHECK_EQ("size() of an empty tray", tray.size(), 0u);
    }

    Tracked::reset();
    {
        Tray tray;
        auto p = std::make_unique<Tracked>(7);
        tray.put(std::move(p));
        CHECK_TRUE("caller's pointer is empty after put(std::move(p))", p == nullptr);
        CHECK_EQ("tray owns the moved-in object", Tracked::alive, 1);
        CHECK_EQ("size() == 1 after moving one in", tray.size(), 1u);
    }
    CHECK_EQ("alive == 0 after scope", Tracked::alive, 0);

    Tracked::reset();
    {
        Tray tray;
        tray.put(std::make_unique<Tracked>(1));
        tray.put(std::make_unique<Tracked>(2));
        auto a = tray.take();
        tray.put(std::make_unique<Tracked>(3));
        auto b = tray.take();
        auto c = tray.take();
        CHECK_EQ("LIFO order: first take", a ? a->value : -1, 2);
        CHECK_EQ("LIFO order: second take", b ? b->value : -1, 3);
        CHECK_EQ("LIFO order: third take", c ? c->value : -1, 1);
        CHECK_TRUE("LIFO order: tray now empty", tray.take() == nullptr);
        CHECK_EQ("no Tracked copies (only pointers moved)", Tracked::copies, 0);
        CHECK_EQ("no Tracked moves (only pointers moved)", Tracked::moves, 0);
    }
    CHECK_EQ("alive == 0 at the end", Tracked::alive, 0);
}
