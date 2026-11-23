#include "check.h"
#include "tracked.h"
#include "solution.cpp"

#include <type_traits>
#include <vector>

void run_tests() {
    {
        IntStack s;
        int out = 42;
        CHECK_EQ("new stack has size 0", s.size(), std::size_t{0});
        CHECK_EQ("new stack has capacity 0", s.capacity(), std::size_t{0});
        CHECK_TRUE("pop on empty returns false", !s.pop(out));
        CHECK_EQ("pop on empty leaves out untouched", out, 42);
    }
    {
        IntStack s;
        for (int v = 1; v <= 5; ++v) s.push(v);
        CHECK_EQ("size == 5 after five pushes", s.size(), std::size_t{5});
        CHECK_EQ("capacity == 8 after five pushes", s.capacity(), std::size_t{8});
        std::vector<int> popped;
        int out = 0;
        while (s.pop(out)) popped.push_back(out);
        CHECK_EQ("pops come out 5,4,3,2,1", popped, (std::vector<int>{5, 4, 3, 2, 1}));
        CHECK_EQ("size == 0 after popping everything", s.size(), std::size_t{0});
        CHECK_EQ("capacity stays 8 after popping everything", s.capacity(), std::size_t{8});
    }
    {
        IntStack s;
        std::vector<std::size_t> caps;
        for (int v = 0; v < 9; ++v) { s.push(v); caps.push_back(s.capacity()); }
        CHECK_EQ("capacity after each push is 1,2,4,4,8,8,8,8,16", caps, (std::vector<std::size_t>{1, 2, 4, 4, 8, 8, 8, 8, 16}));
    }
    {
        IntStack s;
        for (int v = 0; v < 1000; ++v) s.push(v * 3);
        CHECK_EQ("size == 1000 after 1000 pushes", s.size(), std::size_t{1000});
        CHECK_EQ("capacity == 1024 after 1000 pushes", s.capacity(), std::size_t{1024});
        std::vector<int> got;
        int out = 0;
        while (s.pop(out)) got.push_back(out);
        std::vector<int> want;
        for (int v = 999; v >= 0; --v) want.push_back(v * 3);
        CHECK_EQ("1000 values come back in reverse order", got, want);
    }
    {
        IntStack s;
        int out = 0;
        s.push(1);
        s.push(2);
        CHECK_TRUE("pop after two pushes gives 2", s.pop(out) && out == 2);
        s.push(3);
        CHECK_TRUE("pop returns the newest (3)", s.pop(out) && out == 3);
        CHECK_TRUE("pop returns the remaining (1)", s.pop(out) && out == 1);
        CHECK_TRUE("pop on emptied stack is false", !s.pop(out));
        s.push(4);
        CHECK_TRUE("push works after emptying", s.pop(out) && out == 4);
        CHECK_EQ("capacity still 2 after interleaving", s.capacity(), std::size_t{2});
    }
    {
        CHECK_TRUE("IntStack is not copy constructible", !std::is_copy_constructible_v<IntStack>);
        CHECK_TRUE("IntStack is not copy assignable", !std::is_copy_assignable_v<IntStack>);
    }
}
