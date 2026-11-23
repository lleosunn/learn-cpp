#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    {
        Point p(1.5, -2.5);
        CHECK_EQ("constructor stores x", p.x, 1.5);
        CHECK_EQ("constructor stores y", p.y, -2.5);
    }
    {
        Point z{};
        CHECK_EQ("Point{} has x == 0", z.x, 0.0);
        CHECK_EQ("Point{} has y == 0", z.y, 0.0);
    }
    {
        CHECK_NEAR("dist (0,0)->(3,4)", Point(0, 0).dist_to(Point(3, 4)), 5.0, 1e-9);
        CHECK_NEAR("dist (1,1)->(4,5)", Point(1, 1).dist_to(Point(4, 5)), 5.0, 1e-9);
        CHECK_NEAR("dist is symmetric", Point(4, 5).dist_to(Point(1, 1)), 5.0, 1e-9);
    }
    {
        Point p(1, -2);
        Point s = p.scaled(2);
        CHECK_EQ("scaled x", s.x, 2.0);
        CHECK_EQ("scaled y", s.y, -4.0);
        CHECK_EQ("scaled leaves original x alone", p.x, 1.0);
        CHECK_EQ("scaled leaves original y alone", p.y, -2.0);
    }
    {
        Point p(1, 1);
        p.translate(1, 1);
        CHECK_EQ("translate x", p.x, 2.0);
        CHECK_EQ("translate y", p.y, 2.0);
        p.translate(-3, 0.5);
        CHECK_EQ("translate again x", p.x, -1.0);
        CHECK_EQ("translate again y", p.y, 2.5);
    }
    {
        const Point c(1, 1);                    // only const methods compile here
        CHECK_NEAR("const point dist to itself", c.dist_to(c), 0.0, 1e-12);
        Point s = c.scaled(3);
        CHECK_EQ("const point scaled", s.x, 3.0);
    }
}
