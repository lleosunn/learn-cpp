#include <cmath>

struct Point {
    double x = 0, y = 0;

    Point() = default;
    Point(double x, double y) : x(x), y(y) {}   // member x <- parameter x

    double dist_to(const Point& o) const {      // const: may be called on a const Point
        double dx = x - o.x, dy = y - o.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    Point scaled(double k) const {              // returns a new Point, *this untouched
        return Point(x * k, y * k);
    }

    void translate(double dx, double dy) {      // mutates, so not const
        x += dx;
        y += dy;
    }
};
