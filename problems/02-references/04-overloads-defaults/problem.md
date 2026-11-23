# clamp_value Overloads

Python has one `def clamp(x, lo=0, hi=100)` and works out at runtime what `x` is. C++ lets you define several functions with the same name and picks one *at compile time* from the argument types.

## The lesson

**Overloading.** Two functions may share a name if their parameter lists differ:

```cpp
int    area(int side);          // square
int    area(int w, int h);      // rectangle
double area(double radius);     // circle
```

`area(3)` calls the first, `area(3, 4)` the second, `area(2.0)` the third. The compiler looks at the *static* types of the arguments and picks the best match. The return type does not participate: two overloads that differ only in return type are an error.

**Default arguments.** A parameter can carry a default, used when the caller omits it:

```cpp
int clamp_value(int x, int lo = 0, int hi = 100);

clamp_value(150);        // lo = 0, hi = 100
clamp_value(150, 10);    // lo = 10, hi = 100
clamp_value(7, 1, 5);    // nothing defaulted
```

Rules: defaults fill from the right, so once one parameter has a default every parameter after it must too. There are no keyword arguments, so you cannot skip `lo` and supply only `hi`. If you split a function into a declaration (in a header) and a definition (in a .cpp), the defaults go on the declaration only; writing them in both places is a compile error.

**Resolution with `int` vs `double`.** Given both

```cpp
int    clamp_value(int x,    int lo = 0,      int hi = 100);
double clamp_value(double x, double lo = 0.0, double hi = 1.0);
```

`clamp_value(150)` is an exact match for the `int` version and `clamp_value(2.5)` an exact match for the `double` version. The decision is final at compile time; nothing is checked at runtime.

Mixing them is where it bites:

```cpp
clamp_value(5, 0, 1.0);
// error: call to 'clamp_value' is ambiguous
// note: candidate function  int clamp_value(int, int, int)
// note: candidate function  double clamp_value(double, double, double)
```

The `int` overload needs one `double -> int` conversion; the `double` overload needs two `int -> double` conversions. Neither is at least as good on *every* argument, so the compiler refuses to guess. You will meet this error; the fix is to make the argument types agree (`5.0, 0.0, 1.0`).

Why overload instead of `clamp_int`, `clamp_double`, `clamp_float`: the caller writes one name, the compiler verifies that the types line up, and there is no runtime dispatch cost. It also makes templates (chapter 8) feel natural later: a template is a machine for generating overloads.

Two tools for the body: the conditional operator `cond ? a : b` is Python's `a if cond else b`, and `std::clamp(x, lo, hi)` in `<algorithm>` is the production version of this whole problem.

## Coming from Python

- One Python function handles ints and floats because `<` works on both at runtime. C++ needs one overload per type (or a template).
- `clamp(x, hi=5)` has no equivalent. Arguments are positional and defaults fill from the right.
- The *static* type picks the overload. `clamp_value(2.5)` calls the `double` version even if you store the result in an `int`.
- With only the `int` overload defined, `clamp_value(2.5)` still compiles and silently truncates `2.5` to `2` under the flags used here. A missing overload is not an error; it is a quietly wrong answer.

## Your task

Write two overloads. Each returns `x` clamped into `[lo, hi]`.

```cpp
int clamp_value(int x, int lo = 0, int hi = 100);
double clamp_value(double x, double lo = 0.0, double hi = 1.0);
```

| call | result |
|---|---|
| `clamp_value(150)` | `100` |
| `clamp_value(-5)` | `0` |
| `clamp_value(50)` | `50` |
| `clamp_value(7, 1, 5)` | `5` |
| `clamp_value(2.5)` | `1.0` |
| `clamp_value(-0.5)` | `0.0` |
| `clamp_value(0.25, 0.5, 0.75)` | `0.5` |

The starter has only the `int` overload and returns `x` unchanged. Run it first: `clamp_value(2.5)` compiles anyway and yields `2`.
