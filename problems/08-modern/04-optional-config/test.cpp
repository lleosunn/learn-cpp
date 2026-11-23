#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    CHECK_EQ("parse_int(\"42\")", parse_int("42").value_or(-1), 42);
    CHECK_EQ("parse_int(\"-7\")", parse_int("-7").value_or(0), -7);
    CHECK_EQ("parse_int(\"0\")", parse_int("0").value_or(-1), 0);
    CHECK_TRUE("parse_int(\"+5\") rejected", !parse_int("+5").has_value());
    CHECK_TRUE("parse_int(\"\") rejected", !parse_int("").has_value());
    CHECK_TRUE("parse_int(\"12a\") rejected", !parse_int("12a").has_value());
    CHECK_TRUE("parse_int(\" 3\") rejected", !parse_int(" 3").has_value());
    CHECK_TRUE("parse_int(\"-\") rejected", !parse_int("-").has_value());
    CHECK_EQ("parse_int INT_MAX", parse_int("2147483647").value_or(-1), 2147483647);
    CHECK_TRUE("parse_int INT_MAX + 1 rejected", !parse_int("2147483648").has_value());
    CHECK_EQ("parse_int INT_MIN", parse_int("-2147483648").value_or(0), -2147483647 - 1);
    CHECK_TRUE("parse_int 20 digits rejected", !parse_int("99999999999999999999").has_value());

    const std::pair<std::string, int> none{"<none>", 0};
    CHECK_EQ("parse_kv(\"x=1\")", parse_kv("x=1").value_or(none), (std::pair<std::string, int>{"x", 1}));
    CHECK_EQ("parse_kv(\"port=-80\")", parse_kv("port=-80").value_or(none), (std::pair<std::string, int>{"port", -80}));
    CHECK_TRUE("parse_kv(\"=1\") rejected", !parse_kv("=1").has_value());
    CHECK_TRUE("parse_kv(\"x\") rejected", !parse_kv("x").has_value());
    CHECK_TRUE("parse_kv(\"x=y\") rejected", !parse_kv("x=y").has_value());
    CHECK_TRUE("parse_kv(\"x=1=2\") rejected", !parse_kv("x=1=2").has_value());
    CHECK_TRUE("parse_kv(\"x=\") rejected", !parse_kv("x=").has_value());

    std::vector<std::string> lines{"a=1", "bad", "b=2", "a=3", "=9", "c=x"};
    CHECK_EQ("parse_config skips invalid, later wins", parse_config(lines), (std::map<std::string, int>{{"a", 3}, {"b", 2}}));
    std::vector<std::string> empty;
    CHECK_EQ("parse_config empty input", parse_config(empty), (std::map<std::string, int>{}));
}
