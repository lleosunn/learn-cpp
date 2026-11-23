#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    CHECK_EQ("split(\"a,b,,c\", ',')", split("a,b,,c", ','), (std::vector<std::string>{"a", "b", "", "c"}));
    CHECK_EQ("split(\"\", ',')", split("", ','), (std::vector<std::string>{""}));
    CHECK_EQ("split(\",\", ',')", split(",", ','), (std::vector<std::string>{"", ""}));
    CHECK_EQ("split(\"abc\", ',')", split("abc", ','), (std::vector<std::string>{"abc"}));
    CHECK_EQ("split(\"one two\", ' ')", split("one two", ' '), (std::vector<std::string>{"one", "two"}));
    CHECK_EQ("split(\"a,b,\", ',') keeps trailing empty", split("a,b,", ','), (std::vector<std::string>{"a", "b", ""}));

    CHECK_EQ("join({x,y,z}, \", \")", join({"x", "y", "z"}, ", "), std::string("x, y, z"));
    CHECK_EQ("join({}, \",\")", join({}, ","), std::string(""));
    CHECK_EQ("join({solo}, \",\")", join({"solo"}, ","), std::string("solo"));
    CHECK_EQ("join with empty separator", join({"a", "b", "c"}, ""), std::string("abc"));

    CHECK_EQ("to_upper(\"MiXed 1!\")", to_upper("MiXed 1!"), std::string("MIXED 1!"));
    CHECK_EQ("to_upper(\"hello, world\")", to_upper("hello, world"), std::string("HELLO, WORLD"));
    CHECK_EQ("to_upper(\"\")", to_upper(""), std::string(""));

    std::string s = "a,,b,c,";
    CHECK_EQ("round trip join(split(s)) == s", join(split(s, ','), ","), s);
}
