#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    {
        Fraction f(2, 4);
        CHECK_EQ("2/4 reduces: num", f.num(), 1LL);
        CHECK_EQ("2/4 reduces: den", f.den(), 2LL);
    }
    {
        Fraction f(3, -6);
        CHECK_EQ("3/-6 normalizes: num", f.num(), -1LL);
        CHECK_EQ("3/-6 normalizes: den", f.den(), 2LL);
    }
    {
        Fraction f(-2, -4);
        CHECK_EQ("-2/-4 normalizes: num", f.num(), 1LL);
        CHECK_EQ("-2/-4 normalizes: den", f.den(), 2LL);
    }
    {
        Fraction f(0, 5);
        CHECK_EQ("0/5 is 0/1: num", f.num(), 0LL);
        CHECK_EQ("0/5 is 0/1: den", f.den(), 1LL);
        Fraction d;
        CHECK_EQ("default Fraction is 0/1: num", d.num(), 0LL);
        CHECK_EQ("default Fraction is 0/1: den", d.den(), 1LL);
    }
    {
        Fraction s = Fraction(1, 2) + Fraction(1, 3);
        CHECK_EQ("1/2 + 1/3: num", s.num(), 5LL);
        CHECK_EQ("1/2 + 1/3: den", s.den(), 6LL);
        Fraction one = Fraction(1, 3) + Fraction(2, 3);
        CHECK_EQ("1/3 + 2/3: num", one.num(), 1LL);
        CHECK_EQ("1/3 + 2/3: den", one.den(), 1LL);
        Fraction z = Fraction(1, 2) + Fraction(-1, 2);
        CHECK_EQ("1/2 + -1/2: num", z.num(), 0LL);
        CHECK_EQ("1/2 + -1/2: den", z.den(), 1LL);
    }
    {
        CHECK_TRUE("1/2 == 2/4", Fraction(1, 2) == Fraction(2, 4));
        CHECK_TRUE("1/2 != 1/3", !(Fraction(1, 2) == Fraction(1, 3)));
        CHECK_TRUE("-1/2 == 1/-2", Fraction(-1, 2) == Fraction(1, -2));
    }
    {
        std::ostringstream oss;
        oss << Fraction(-1, 2) << " " << Fraction(3);
        CHECK_EQ("<< chains and prints n/d", oss.str(), std::string("-1/2 3/1"));
    }
    {
        std::ostringstream oss;
        oss << (Fraction(1, 6) + Fraction(1, 3));
        CHECK_EQ("<< prints a sum", oss.str(), std::string("1/2"));
    }
}
