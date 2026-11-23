#include "check.h"
#include "tracked.h"
#include "solution.cpp"

#include <type_traits>
#include <utility>
#include <vector>

void run_tests() {
    {
        IntBuffer a(4, 7);
        const int* before = a.data();
        IntBuffer b(std::move(a));
        CHECK_TRUE("move ctor: b owns a's old block (no copy)", b.data() == before);
        CHECK_EQ("move ctor: b.size()", b.size(), 4u);
        CHECK_EQ("move ctor: b[3] keeps the value", b[3], 7);
        CHECK_EQ("move ctor: a.size() == 0 afterwards", a.size(), 0u);
        CHECK_TRUE("move ctor: a.data() == nullptr afterwards", a.data() == nullptr);
    }   // forgetting to null the source: ASan double-free here

    {
        IntBuffer a(3, 4);
        IntBuffer b(5, 1);
        const int* before = a.data();
        b = std::move(a);
        CHECK_TRUE("move assign: b took a's block", b.data() == before);
        CHECK_EQ("move assign: b.size()", b.size(), 3u);
        CHECK_EQ("move assign: b[2]", b[2], 4);
        CHECK_EQ("move assign: a.size() == 0 afterwards", a.size(), 0u);
        CHECK_TRUE("move assign: a.data() == nullptr afterwards", a.data() == nullptr);
    }

    {
        IntBuffer a(4, 7);
        IntBuffer& alias = a;
        a = std::move(alias);   // self-move through an alias
        CHECK_TRUE("self-move leaves a valid (unchanged or empty, never dangling)",
                   (a.size() == 4 && a.data() != nullptr && a[0] == 7) || (a.size() == 0 && a.data() == nullptr));
        a = IntBuffer(2, 5);
        CHECK_EQ("assignable after self-move: size", a.size(), 2u);
        CHECK_EQ("assignable after self-move: value", a[1], 5);
    }

    {
        IntBuffer a(2, 9);
        IntBuffer b(std::move(a));
        a = IntBuffer(3, 1);    // a moved-from object must accept a new value
        CHECK_EQ("moved-from a reassigned: size", a.size(), 3u);
        CHECK_EQ("moved-from a reassigned: a[2]", a[2], 1);
        CHECK_EQ("b unaffected by reassigning a", b[1], 9);
    }

    {
        std::vector<IntBuffer> v;
        v.reserve(1);
        v.push_back(IntBuffer(2, 1));
        const int* keep = v[0].data();
        v.push_back(IntBuffer(3, 2));            // reallocation: vector moves only if the move ctor is noexcept
        CHECK_TRUE("vector growth moved v[0] instead of copying it (needs noexcept)", v[0].data() == keep);
        CHECK_EQ("vector growth kept v[0]'s contents", v[0][1], 1);
        CHECK_EQ("vector growth kept v[1]'s contents", v[1][2], 2);
        CHECK_TRUE("is_nothrow_move_constructible<IntBuffer>", std::is_nothrow_move_constructible_v<IntBuffer>);
        CHECK_TRUE("is_nothrow_move_assignable<IntBuffer>", std::is_nothrow_move_assignable_v<IntBuffer>);
    }

    {
        IntBuffer a(3, 1);
        IntBuffer b = a;
        b[0] = 99;
        CHECK_EQ("copy is still deep: a[0] unchanged", a[0], 1);
        CHECK_TRUE("copy is still deep: different blocks", a.data() != b.data());
        IntBuffer c(1, 0);
        c = a;
        CHECK_EQ("copy assign still deep: size", c.size(), 3u);
        CHECK_TRUE("copy assign still deep: different blocks", c.data() != a.data());
    }
}
