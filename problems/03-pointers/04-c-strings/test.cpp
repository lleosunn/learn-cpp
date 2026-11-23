#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    CHECK_EQ("c_strlen(\"hello\")", c_strlen("hello"), std::size_t{5});
    CHECK_EQ("c_strlen(\"\")", c_strlen(""), std::size_t{0});
    CHECK_EQ("c_strlen(\"a\")", c_strlen("a"), std::size_t{1});
    CHECK_EQ("c_strlen(\"ab\\0cd\") stops at the embedded NUL", c_strlen("ab\0cd"), std::size_t{2});

    const char* s = "hello";
    CHECK_TRUE("find_char(s, 'h') == s with s = \"hello\"", find_char(s, 'h') == s);
    CHECK_TRUE("find_char(s, 'l') == s + 2 (the first l)", find_char(s, 'l') == s + 2);
    CHECK_TRUE("find_char(s, 'o') == s + 4", find_char(s, 'o') == s + 4);
    CHECK_TRUE("find_char(s, 'z') == nullptr", find_char(s, 'z') == nullptr);
    CHECK_TRUE("find_char(s, '\\0') == s + 5 (the terminator)", find_char(s, '\0') == s + 5);

    const char* e = "";
    CHECK_TRUE("find_char(\"\", 'a') == nullptr", find_char(e, 'a') == nullptr);
    CHECK_TRUE("find_char(\"\", '\\0') == the string itself", find_char(e, '\0') == e);
}
