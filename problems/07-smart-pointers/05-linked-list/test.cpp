#include "check.h"
#include "tracked.h"
#include "solution.cpp"

#include <type_traits>
#include <vector>

void run_tests() {
    {
        List l;
        CHECK_EQ("empty list: size()", l.size(), 0u);
        CHECK_EQ("empty list: to_vector()", l.to_vector(), std::vector<int>{});
        int out = -1;
        CHECK_TRUE("pop_front on empty returns false", !l.pop_front(out));
        CHECK_EQ("pop_front on empty leaves out untouched", out, -1);
    }

    {
        List l;
        l.push_front(3);
        l.push_front(2);
        l.push_front(1);
        CHECK_EQ("push_front x3: size()", l.size(), 3u);
        CHECK_EQ("push_front x3: to_vector()", l.to_vector(), (std::vector<int>{1, 2, 3}));
    }

    {
        List l;
        l.push_back(1);
        l.push_back(2);
        l.push_back(3);
        CHECK_EQ("push_back x3: size()", l.size(), 3u);
        CHECK_EQ("push_back x3: to_vector()", l.to_vector(), (std::vector<int>{1, 2, 3}));
        l.push_front(0);
        CHECK_EQ("mixed push_front/push_back", l.to_vector(), (std::vector<int>{0, 1, 2, 3}));
    }

    {
        List l;
        l.push_back(1);
        l.push_back(2);
        l.push_back(3);
        int a = 0, b = 0, c = 0, d = -1;
        bool oka = l.pop_front(a), okb = l.pop_front(b), okc = l.pop_front(c);
        bool okd = l.pop_front(d);
        CHECK_TRUE("pop_front x3 all succeed", oka && okb && okc);
        CHECK_EQ("pop_front order", (std::vector<int>{a, b, c}), (std::vector<int>{1, 2, 3}));
        CHECK_TRUE("fourth pop_front fails", !okd);
        CHECK_EQ("fourth pop_front leaves out untouched", d, -1);
        CHECK_EQ("size() after draining", l.size(), 0u);
        l.push_back(4);
        CHECK_EQ("usable after draining", l.to_vector(), std::vector<int>{4});
    }

    {
        List l;
        l.push_back(1);
        l.push_back(2);
        l.push_back(3);
        l.reverse();
        CHECK_EQ("reverse {1,2,3}", l.to_vector(), (std::vector<int>{3, 2, 1}));
        CHECK_EQ("reverse keeps size()", l.size(), 3u);
        l.reverse();
        CHECK_EQ("reverse twice is identity", l.to_vector(), (std::vector<int>{1, 2, 3}));

        List e;
        e.reverse();
        CHECK_EQ("reverse on empty", e.to_vector(), std::vector<int>{});

        List s;
        s.push_back(5);
        s.reverse();
        CHECK_EQ("reverse on a single element", s.to_vector(), std::vector<int>{5});
    }

    {
        List l;
        std::vector<int> expect;
        for (int i = 999; i >= 0; --i) l.push_front(i);
        for (int i = 0; i < 1000; ++i) expect.push_back(i);
        CHECK_EQ("1000 push_front: size()", l.size(), 1000u);
        CHECK_TRUE("1000 push_front: to_vector() is 0..999", l.to_vector() == expect);
        l.reverse();
        int first = -1;
        l.pop_front(first);
        CHECK_EQ("1000 nodes reversed: first is 999", first, 999);
    }   // recursive destruction of ~1000 nodes: fine at this size

    CHECK_TRUE("List is not copy constructible (unique_ptr member)", !std::is_copy_constructible_v<List>);
    CHECK_TRUE("List is not copy assignable", !std::is_copy_assignable_v<List>);
    CHECK_TRUE("List is nothrow move constructible (rule of zero)", std::is_nothrow_move_constructible_v<List>);
    CHECK_TRUE("List is nothrow move assignable (rule of zero)", std::is_nothrow_move_assignable_v<List>);
    {
        List a;
        a.push_back(1);
        a.push_back(2);
        List b = std::move(a);
        CHECK_EQ("moved-to list has the nodes", b.to_vector(), (std::vector<int>{1, 2}));
        CHECK_EQ("moved-to list has the size", b.size(), 2u);
    }
}
