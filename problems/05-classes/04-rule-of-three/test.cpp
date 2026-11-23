#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    {
        IntBuffer a(3, 7);
        IntBuffer b = a;          // copy constructor
        b[0] = 99;
        CHECK_EQ("copy: a[0] unchanged after writing b[0]", a[0], 7);
        CHECK_EQ("copy: b[0] took the write", b[0], 99);
        CHECK_EQ("copy: size carried over", b.size(), std::size_t{3});
        CHECK_EQ("copy: other elements carried over", b[2], 7);
    }   // both destructors run here: shallow copy double-frees
    {
        IntBuffer a(3, 1);
        IntBuffer b(5, 2);
        a = b;                    // copy assignment across sizes
        CHECK_EQ("assign: size follows source", a.size(), std::size_t{5});
        CHECK_EQ("assign: values follow source", a[4], 2);
        a[4] = 42;
        CHECK_EQ("assign: target write does not leak into source", b[4], 2);
        b[0] = -1;
        CHECK_EQ("assign: source write does not leak into target", a[0], 2);
    }
    {
        IntBuffer a(4, 9);
        IntBuffer& r = a;         // alias, so this is a = a without the compiler noticing
        a = r;
        CHECK_EQ("self-assign: size intact", a.size(), std::size_t{4});
        CHECK_EQ("self-assign: a[0] intact", a[0], 9);
        CHECK_EQ("self-assign: a[3] intact", a[3], 9);
    }
    {
        IntBuffer a(1, 0), b(2, 0), c(3, 5);
        a = b = c;                // needs operator= to return *this
        CHECK_EQ("chain: a.size()", a.size(), std::size_t{3});
        CHECK_EQ("chain: b.size()", b.size(), std::size_t{3});
        CHECK_EQ("chain: a[2]", a[2], 5);
        a[1] = 1;
        b[1] = 2;
        CHECK_EQ("chain: c independent of a and b", c[1], 5);
        CHECK_EQ("chain: b independent of a", b[1], 2);
    }
    {
        IntBuffer a(3, 4);
        const IntBuffer& cr = a;  // only the const operator[] is callable here
        CHECK_EQ("const operator[] reads", cr[1], 4);
        a[1] = 8;
        CHECK_EQ("const view sees the write", cr[1], 8);
    }
    {
        IntBuffer keep(1);
        {
            IntBuffer tmp(2, 5);
            keep = tmp;
        }                         // tmp is gone; keep must own its own array
        CHECK_EQ("copy outlives original: size", keep.size(), std::size_t{2});
        CHECK_EQ("copy outlives original: value", keep[1], 5);
    }
}
