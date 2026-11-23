# Virtual Dispatch

In Python, calling `shape.area()` runs whichever `area` the object's actual class defines. Always. C++ does that only when you ask for it with `virtual`, and when you forget, the code compiles, runs, and quietly calls the wrong function. The starter for this problem is that silent failure.

## The lesson

**Inheritance.** `class Circle : public Shape` makes `Circle` a kind of `Shape`: a `Shape*` or `Shape&` may refer to a `Circle`. The base constructor runs first (implicitly here, since `Shape` has nothing to initialize), then the derived members.

**Static versus dynamic dispatch.** By default, C++ chooses which function to call from the *static* type, the type written in the source. Given `const Shape* s = &c;`, the call `s->area()` means `Shape::area`, no matter what `s` points at. `Circle::area` with the same name and signature does not override the base version; it *hides* it, and is used only when the static type is `Circle`.

Run the starter before editing it. `c.area()` is right, because `c` is a `Circle`. `total_area` loops over `const Shape*` and sums `Shape::area`, so it returns 0. `name()` through a `Shape&` returns `"shape"`. No warning, no error, wrong answers.

Adding `virtual` to the declaration in the base class switches that function to *dynamic* dispatch: the call looks at the object's real type at runtime (through a hidden pointer to a table of function pointers, the vtable) and calls the most-derived version. You pay one pointer per object and one indirect call, and only for the functions you mark `virtual`.

```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual std::string name() const { return "shape"; }
};

class Circle : public Shape {
public:
    double area() const override { return kPi * r_ * r_; }
    std::string name() const override { return "circle"; }
    // ...
};
```

Three pieces, each with its own reason:

- **`override`** on the derived declaration asks the compiler to verify that this function really overrides a virtual in the base. Without it, `std::string name()` (missing `const`) declares a new, unrelated function that hides the base one; `c.name()` still says `"circle"`, but through a `Shape&` you are back to `"shape"`, and the compiler at most warns. With `override` it is a hard error: `non-virtual member function marked 'override' hides virtual member function`. Always write it.
- **`= 0`** makes `area` *pure virtual*: `Shape` provides no body, and becomes an **abstract class** that cannot be instantiated. A derived class must implement every pure virtual to be instantiable itself. This encodes "a generic shape has no area" in the type system instead of returning a made-up 0. `name()` keeps a body, so derived classes may inherit it.
- **The virtual destructor.** `delete p` where `p` is a `Shape*` pointing at a `Circle` runs `~Shape` only, unless `~Shape` is virtual. Members of the derived object are never destroyed, and the language calls it undefined behavior. The test checks this with a `Probe` type whose destructor sets a flag: with a non-virtual base destructor, the flag stays 0. The rule is mechanical: **any class with a virtual function gets `virtual ~Base() = default;`**. Clang helps once `area()` is virtual: it warns on the test's `delete p` with `-Wdelete-abstract-non-virtual-dtor` (part of the `-Wdelete-non-virtual-dtor` family) until the destructor is virtual too.

**Why pointers in the vector?** A `std::vector<Shape>` would store `Shape` objects by value, and copying a `Circle` into a `Shape` slot keeps only the `Shape` part: the radius is sliced off, and `Shape` is abstract anyway. Polymorphism needs indirection, so the containers here hold `const Shape*`. Chapter 7 upgrades that to `std::unique_ptr<Shape>`.

## Coming from Python

- Every Python method is virtual; overriding is just defining the same name. In C++ the base must say `virtual`, or the derived definition hides instead of overrides.
- `abc.ABC` plus `@abstractmethod` is `= 0`, except the check happens at compile time, not when you try to instantiate.
- `super().__init__(...)` is the base-class entry in the initializer list, `Circle(double r) : Shape(), r_(r)`. It is implicit when the base is default-constructible.
- Python objects always live behind references, so there is no slicing. C++ objects are values unless you hold them through a pointer or reference.
- `isinstance` chains are a smell in both languages; virtual dispatch is the replacement.

## Your task

Make `Shape` a proper abstract base class with virtual dispatch and a virtual destructor, mark the derived methods `override`, and implement `largest_name`.

```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual std::string name() const;        // "shape"
};
class Circle : public Shape {
public:
    explicit Circle(double r);
    double area() const override;
    std::string name() const override;       // "circle"
};
class Rect : public Shape {
public:
    Rect(double w, double h);
    double area() const override;
    std::string name() const override;       // "rect"
};
double total_area(const std::vector<const Shape*>& shapes);
std::string largest_name(const std::vector<const Shape*>& shapes);   // "" if empty
```

| call | result |
|---|---|
| `Circle(1).area()` | `3.14159...` |
| `Rect(2, 3).area()` | `6` |
| `total_area({&c, &r})` with the two above | `9.14159...` (starter: `0`) |
| `largest_name({&c, &r})` | `"rect"` (empty vector: `""`) |
| `const Shape& s = c; s.name()` | `"circle"` (starter: `"shape"`) |
| `Shape* p = new Probe(flag); delete p;` | `~Probe` runs, `flag == 1` (starter: `0`) |
