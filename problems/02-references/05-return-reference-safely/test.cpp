#include "check.h"
#include "tracked.h"
#include "solution.cpp"

#include <string>
#include <vector>

void run_tests() {
    {
        std::vector<std::string> v = {"a", "bbb", "cc"};
        CHECK_EQ("longest_word({\"a\", \"bbb\", \"cc\"})", longest_word(v), std::string("bbb"));
        CHECK_TRUE("longest_word returns a reference into the vector: &longest_word(v) == &v[1]", &longest_word(v) == &v[1]);
    }
    {
        std::vector<std::string> v = {"aa", "bb"};
        CHECK_EQ("longest_word({\"aa\", \"bb\"}) first wins ties", longest_word(v), std::string("aa"));
        CHECK_TRUE("tie: &longest_word(v) == &v[0]", &longest_word(v) == &v[0]);
    }
    {
        std::vector<std::string> v = {"x"};
        CHECK_EQ("longest_word({\"x\"})", longest_word(v), std::string("x"));
        CHECK_TRUE("single element: &longest_word(v) == &v[0]", &longest_word(v) == &v[0]);
    }
    {
        std::vector<std::string> v = {"", "abc", "abcd", "ab"};
        CHECK_EQ("longest_word({\"\", \"abc\", \"abcd\", \"ab\"})", longest_word(v), std::string("abcd"));
        CHECK_TRUE("&longest_word(v) == &v[2]", &longest_word(v) == &v[2]);
    }
    {
        std::vector<std::string> v = {"short", std::string(100, 'z'), "also short"};
        CHECK_EQ("longest_word with a 100-char heap-allocated string", longest_word(v).size(), std::size_t(100));
        CHECK_TRUE("long string: &longest_word(v) == &v[1]", &longest_word(v) == &v[1]);
    }
}
