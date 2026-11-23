#include <string>
#include <vector>

constexpr double kPi = 3.14159265358979323846;

// TODO: nothing here is virtual yet. This compiles and runs. Run the tests
// first and look at what total_area and name() return when the call goes
// through a Shape pointer or reference.
class Shape {
public:
    ~Shape() = default;
    double area() const { return 0; }
    std::string name() const { return "shape"; }
};

class Circle : public Shape {
public:
    explicit Circle(double r) : r_(r) {}
    double area() const { return kPi * r_ * r_; }   // hides Shape::area, does not override it
    std::string name() const { return "circle"; }
private:
    double r_;
};

class Rect : public Shape {
public:
    Rect(double w, double h) : w_(w), h_(h) {}
    double area() const { return w_ * h_; }
    std::string name() const { return "rect"; }
private:
    double w_, h_;
};

double total_area(const std::vector<const Shape*>& shapes) {
    double sum = 0;
    for (const Shape* s : shapes) sum += s->area();
    return sum;
}

// TODO: name of the shape with the largest area, "" if the vector is empty
std::string largest_name(const std::vector<const Shape*>& shapes) {
    return "";
}
