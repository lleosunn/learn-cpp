#include "check.h"
#include "tracked.h"
#include "solution.cpp"

#include <algorithm>
#include <cstring>

// The chars in buf up to its terminator, never reading past cap (so a missing '\0' shows as trailing x's).
static std::string text(const char* buf, std::size_t cap) { return std::string(buf, std::find(buf, buf + cap, '\0')); }

void run_tests() {
    char b6[6];
    std::memset(b6, 'x', sizeof b6);
    CHECK_EQ("copy_string(b6, 6, \"hello\") returns 5 (exact fit)", copy_string(b6, 6, "hello"), std::size_t{5});
    CHECK_EQ("b6 holds \"hello\" with its terminator at b6[5]", text(b6, 6), std::string("hello"));

    std::memset(b6, 'x', sizeof b6);
    CHECK_EQ("copy_string(b6, 6, \"hello world\") returns 5 (truncated)", copy_string(b6, 6, "hello world"), std::size_t{5});
    CHECK_EQ("b6 holds \"hello\" after the truncated copy", text(b6, 6), std::string("hello"));

    char b4[4];
    std::memset(b4, 'x', sizeof b4);
    CHECK_EQ("copy_string(b4, 4, \"hello\") returns 3", copy_string(b4, 4, "hello"), std::size_t{3});
    CHECK_EQ("b4 holds \"hel\"", text(b4, 4), std::string("hel"));

    char b8[8];
    std::memset(b8, 'x', sizeof b8);
    CHECK_EQ("copy_string(b8, 8, \"hi\") returns 2 (plenty of room)", copy_string(b8, 8, "hi"), std::size_t{2});
    CHECK_EQ("b8 holds \"hi\" (stopped at src's terminator)", text(b8, 8), std::string("hi"));

    char b1[1] = {'x'};
    CHECK_EQ("copy_string(b1, 1, \"hello\") returns 0", copy_string(b1, 1, "hello"), std::size_t{0});
    CHECK_TRUE("b1[0] == '\\0' (only the terminator fits)", b1[0] == '\0');

    std::memset(b6, 'x', sizeof b6);
    CHECK_EQ("copy_string(b6, 6, \"\") returns 0", copy_string(b6, 6, ""), std::size_t{0});
    CHECK_EQ("b6 holds \"\" after copying an empty string", text(b6, 6), std::string(""));

    char guard[2] = {'x', 'x'};
    CHECK_EQ("copy_string(guard, 0, \"hello\") returns 0", copy_string(guard, 0, "hello"), std::size_t{0});
    CHECK_TRUE("guard[0] is still 'x' (cap == 0 writes nothing)", guard[0] == 'x');
    CHECK_EQ("copy_string(nullptr, 0, \"hello\") returns 0 without crashing", copy_string(nullptr, 0, "hello"), std::size_t{0});
}
