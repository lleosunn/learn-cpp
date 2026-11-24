#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    // Separate exact-size stack buffers: identical contents never share an address,
    // and reading past a terminator is an ASan report.
    char x[] = "hello";
    char y[] = "hello";
    char help[] = "help!";
    char hell[] = "hell";
    char cap[] = "Hello";
    char e1[] = "";
    char e2[] = "";

    CHECK_EQ("c_streq(x, y) with two separate buffers both holding \"hello\"", c_streq(x, y), true);
    CHECK_EQ("c_streq(x, x)", c_streq(x, x), true);
    CHECK_EQ("c_streq(\"hello\", \"help!\") (same length, differ at index 3)", c_streq(x, help), false);
    CHECK_EQ("c_streq(\"hello\", \"hell\") (second is a prefix of the first)", c_streq(x, hell), false);
    CHECK_EQ("c_streq(\"hell\", \"hello\") (first is a prefix of the second)", c_streq(hell, x), false);
    CHECK_EQ("c_streq(\"hello\", \"Hello\") (case-sensitive)", c_streq(x, cap), false);
    CHECK_EQ("c_streq(\"\", \"\") with two separate empty buffers", c_streq(e1, e2), true);
    CHECK_EQ("c_streq(\"\", \"hello\")", c_streq(e1, x), false);
    CHECK_EQ("c_streq(\"hello\", \"\")", c_streq(x, e1), false);

    char he[] = "he";
    char hex[] = "hex";
    char el[] = "el";
    char a[] = "a";
    CHECK_EQ("starts_with(\"hello\", \"he\")", starts_with(x, he), true);
    CHECK_EQ("starts_with(\"hello\", \"hello\") (a string starts with itself)", starts_with(x, y), true);
    CHECK_EQ("starts_with(\"hello\", \"\") (empty prefix)", starts_with(x, e1), true);
    CHECK_EQ("starts_with(\"\", \"\")", starts_with(e1, e2), true);
    CHECK_EQ("starts_with(\"he\", \"hello\") (prefix longer than the string)", starts_with(he, x), false);
    CHECK_EQ("starts_with(\"hello\", \"hex\")", starts_with(x, hex), false);
    CHECK_EQ("starts_with(\"hello\", \"el\") (present, but not at the start)", starts_with(x, el), false);
    CHECK_EQ("starts_with(\"\", \"a\")", starts_with(e1, a), false);
}
