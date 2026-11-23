#include <string>
#include <vector>

constexpr double kPi = 3.14159265358979323846;

class Shape {
public:
    virtual ~Shape() = default;          // virtual: delete through Shape* runs the derived destructor
    virtual double area() const = 0;     // pure virtual: Shape is abstract
    virtual std::string name() const { return "shape"; }   // virtual with a default body
};

class Circle : public Shape {
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return kPi * r_ * r_; }   // override: compiler checks the signature
    std::string name() const override { return "circle"; }
private:
    double r_;
};

class Rect : public Shape {
public:
    Rect(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    std::string name() const override { return "rect"; }
private:
    double w_, h_;
};

double total_area(const std::vector<const Shape*>& shapes) {
    double sum = 0;
    for (const Shape* s : shapes) sum += s->area();   // dispatched on the dynamic type
    return sum;
}

std::string largest_name(const std::vector<const Shape*>& shapes) {
    const Shape* best = nullptr;
    for (const Shape* s : shapes)
        if (!best || s->area() > best->area()) best = s;
    return best ? best->name() : "";
}
