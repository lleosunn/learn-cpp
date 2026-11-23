# Point with Const Methods

A C++ class is data plus the functions that operate on it, with the compiler enforcing which of those functions is allowed to change the data. This problem introduces constructors, member functions, and the `const` qualifier that separates readers from writers.

## The lesson

Here is a Python class and its C++ equivalent:

```python
class Point:
    def __init__(self, x=0.0, y=0.0):
        self.x = x
        self.y = y

    def dist_to(self, o):
        return math.hypot(self.x - o.x, self.y - o.y)
```

```cpp
struct Point {
    double x = 0, y = 0;                        // members, with default values

    Point() = default;                          // no-arg constructor: "just use the defaults"
    Point(double x, double y) : x(x), y(y) {}   // two-arg constructor

    double dist_to(const Point& o) const {      // a const member function
        double dx = x - o.x, dy = y - o.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};
```

Things to notice:

- **`struct` vs `class`.** They are the same thing except for the default access: `struct` members are public unless you say otherwise, `class` members are private. Use `struct` for plain data bundles like a point.
- **Constructors** have the class's name and no return type. You can have several with different parameter lists. The moment you declare *any* constructor, the compiler stops generating the no-argument one for you, so `Point() = default;` asks for it back.
- **The member initializer list** is the `: x(x), y(y)` part. It runs before the body and is where members get their initial values. In `x(x)`, the name outside the parentheses is always the member; the name inside is looked up normally and finds the parameter. This is the idiomatic way to initialize members.
- **Members are in scope inside member functions.** A bare `x` means "this object's `x`". The current object is reachable through the pointer `this` (`this->x`), but you almost never need to write it.
- **`const` member functions.** The `const` after `dist_to`'s parameter list is a promise: this method does not modify `*this`. Inside a `const` method, every member is read-only, and trying to assign to one is a compile error.

Why `const` matters: you normally pass objects as `const Point&` (chapter 2). On a `const` reference you may only call `const` methods. Forget the `const` on `dist_to` and nobody who received a `const Point&` can measure a distance; they get `'this' argument to member function 'dist_to' has type 'const Point', but function is not marked const` (GCC words the same error as `discards qualifiers`). The rule: if a method does not change the object, mark it `const`.

A pitfall the starter demonstrates on purpose: inside a constructor *body*, `x = x;` assigns the parameter to itself. The member keeps its default of 0, and clang tells you so with `-Wself-assign`. Run the starter before you edit it and read that warning. The initializer list has no such ambiguity, which is one more reason to prefer it.

## Coming from Python

- There is no `self`. Members are simply in scope inside methods; `this` is the hidden pointer that makes that work.
- `__init__` becomes one or more constructors, and they can be overloaded by parameter list.
- Python has no notion of a read-only method; anything can mutate anything. `const` is a contract the compiler checks for you.
- Objects are values. `Point p = q;` makes a copy, and `p.x = 5` leaves `q` alone. In Python that line would create an alias.
- `Point(1, 2)` and `Point{1, 2}` both construct a point; `Point{}` uses the defaults.

## Your task

Complete the `Point` struct:

- `Point(double x, double y)` stores both coordinates.
- `dist_to(o)` returns the Euclidean distance to `o`.
- `scaled(k)` returns a **new** point with both coordinates multiplied by `k`; the original is untouched.
- `translate(dx, dy)` shifts this point in place.

`dist_to` and `scaled` must be callable on a `const Point`.

```cpp
struct Point {
    double x = 0, y = 0;
    Point() = default;
    Point(double x, double y);
    double dist_to(const Point& o) const;
    Point scaled(double k) const;
    void translate(double dx, double dy);
};
```

| call | result |
|---|---|
| `Point(0, 0).dist_to(Point(3, 4))` | `5.0` |
| `Point(1, -2).scaled(2)` | `(2, -4)` |
| `Point p(1, 1); p.translate(1, 1);` | `p` is `(2, 2)` |
| `Point{}` | `(0, 0)` |
| `const Point c(1, 1); c.dist_to(c)` | `0.0` (only compiles if `dist_to` is `const`) |
