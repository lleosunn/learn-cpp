#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    CHECK_EQ("make_adder(5)(3)", make_adder(5)(3), 8);
    CHECK_EQ("make_adder(-2)(2)", make_adder(-2)(2), 0);

    {
        auto add2 = make_adder(2);
        auto add10 = make_adder(10);
        CHECK_EQ("adders are independent: add2(1)", add2(1), 3);
        CHECK_EQ("adders are independent: add10(1)", add10(1), 11);
        CHECK_EQ("adder is reusable", add2(5), 7);
    }

    {
        auto c = make_counter();
        CHECK_EQ("counter first call", c(), 1);
        CHECK_EQ("counter second call", c(), 2);
        CHECK_EQ("counter third call", c(), 3);
        auto d = make_counter();
        CHECK_EQ("second counter starts fresh", d(), 1);
        CHECK_EQ("first counter unaffected", c(), 4);
    }

    {
        std::vector<int> sink;
        auto f = make_appender(sink, 7);
        f();
        f();
        CHECK_EQ("appender pushes into caller's vector", sink, (std::vector<int>{7, 7}));
        auto g = make_appender(sink, 9);
        g();
        CHECK_EQ("two appenders share the sink", sink, (std::vector<int>{7, 7, 9}));
    }
}
