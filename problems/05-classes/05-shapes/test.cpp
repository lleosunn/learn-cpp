#include "check.h"
#include "tracked.h"
#include "solution.cpp"

// A Shape whose destructor reports that it ran. No `override` here on purpose,
// so this file also compiles against the non-virtual starter.
struct Probe : Shape {
    int& flag;
    explicit Probe(int& f) : flag(f) {}
    ~Probe() { flag = 1; }
    double area() const { return 0; }
};

void run_tests() {
    Circle c(1);
    Rect r(2, 3);
    {
        CHECK_NEAR("Circle(1).area()", c.area(), 3.14159265, 1e-6);
        CHECK_NEAR("Rect(2,3).area()", r.area(), 6.0, 1e-12);
        CHECK_EQ("Circle::name()", c.name(), std::string("circle"));
        CHECK_EQ("Rect::name()", r.name(), std::string("rect"));
    }
    {
        std::vector<const Shape*> v{&c, &r};
        CHECK_NEAR("total_area through Shape*", total_area(v), 9.14159265, 1e-6);
        CHECK_EQ("largest_name picks the rect", largest_name(v), std::string("rect"));
        std::vector<const Shape*> rev{&r, &c};
        CHECK_EQ("largest_name independent of order", largest_name(rev), std::string("rect"));
    }
    {
        Rect big(10, 10);
        std::vector<const Shape*> v{&c, &big, &r};
        CHECK_NEAR("total_area of three", total_area(v), 109.14159265, 1e-6);
        CHECK_EQ("largest_name among three", largest_name(v), std::string("rect"));
        std::vector<const Shape*> one{&c};
        CHECK_EQ("largest_name of one", largest_name(one), std::string("circle"));
        CHECK_EQ("largest_name of none", largest_name({}), std::string(""));
        CHECK_NEAR("total_area of none", total_area({}), 0.0, 1e-12);
    }
    {
        const Shape& s = c;
        CHECK_EQ("name() through Shape& dispatches to Circle", s.name(), std::string("circle"));
        CHECK_NEAR("area() through Shape& dispatches to Circle", s.area(), 3.14159265, 1e-6);
    }
    {
        int flag = 0;
        Probe p(flag);
        CHECK_EQ("Shape::name() default is \"shape\"", p.name(), std::string("shape"));
    }
    {
        int flag = 0;
        Shape* p = new Probe(flag);
        delete p;   // only a virtual destructor reaches ~Probe
        CHECK_EQ("delete through Shape* runs the derived destructor", flag, 1);
    }
}
