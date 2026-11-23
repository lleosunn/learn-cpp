#include <cmath>

struct Point {
    double x = 0, y = 0;

    Point() = default;

    // TODO: store the parameters in the members. This version compiles but is
    // wrong: run it first and read the warning the compiler gives you.
    Point(double x, double y) {
        x = x;
        y = y;
    }

    // TODO: Euclidean distance to o
    double dist_to(const Point& o) const {
        return 0;
    }

    // TODO: a new point with both coordinates multiplied by k
    Point scaled(double k) const {
        return Point{};
    }

    // TODO: shift this point in place
    void translate(double dx, double dy) {
    }
};
