# Average of Three

Python has one `/` and it always gives you a float. C++ has one `/` too, but what it does depends on the types of its operands, and it will not tell you which one you got.

## The lesson

C++ has separate types for integers and floating-point numbers, and arithmetic is done **in the type of the operands**, never in the type of the destination:

```cpp
int a = 5, b = 3;
double x = a / b;   // x == 1.0, not 1.6667
```

Both operands are `int`, so `/` is integer division and produces the `int` `1`. Only *then* is `1` converted to `double`, because it is being stored in a `double`. The fractional part was thrown away before the conversion had any say.

The fix is to make at least one operand a `double` before the division:

```cpp
double x = a / 3.0;                       // a literal with a decimal point is a double
double y = static_cast<double>(a) / b;    // explicit conversion of one operand
double z = static_cast<double>(a / b);    // WRONG: converts the already-truncated result
```

The rule C++ applies is called the *usual arithmetic conversions*: if either operand of `+ - * /` is a `double`, the other is promoted to `double` and the operation happens in `double`. If both are `int`, it happens in `int`. The rule is applied one operator at a time, so an `int / int` buried inside a bigger `double` expression is still an integer division.

`static_cast<T>(expr)` is the C++ way to spell a conversion you mean. It is deliberately long so it stands out when you read code and is easy to grep for. Older code uses C-style casts like `(double)a`; prefer `static_cast`, which refuses the genuinely dangerous conversions the C-style cast performs without complaint.

Why this matters: nothing warns you. The program compiles, runs, and produces a plausible-looking number that is simply wrong. A large share of C++ bugs look exactly like this: legal code with a silent type conversion in the middle. Reading an expression and knowing the type of every subexpression is a skill you will use every day.

Two more facts about `double`:

- The literal `3` is an `int`, `3.0` is a `double`, `3.0f` is a `float`. Use `double` unless you have a reason not to.
- Comparing floating-point values with `==` is as fragile as in Python. The tests compare with a tolerance.

## Coming from Python

- Python: `5 / 3 == 1.6667` and `5 // 3 == 1`. C++: `5 / 3 == 1` and `5 / 3.0 == 1.6667`. There is no `//`; integer `/` already truncates (toward zero, not toward minus infinity like Python's `//`).
- Python's `float` is a C `double`. C++ also has `float` (32-bit, about 7 significant digits); avoid it unless memory matters.
- Python converts at the last moment (`int + float` gives `float`). C++ does the same, but per operation, so the conversion can happen too late to help you.
- `int(x)` in Python becomes `static_cast<int>(x)` in C++. Same truncation toward zero.

## Your task

Return the arithmetic mean of three integers as a `double`.

```cpp
double average(int a, int b, int c);
```

| call | result |
|---|---|
| `average(1, 2, 3)` | `2.0` |
| `average(1, 2, 2)` | `1.6667` |
| `average(0, 0, 1)` | `0.3333` |
| `average(-1, -2, -3)` | `-2.0` |
| `average(-1, 0, 0)` | `-0.3333` |

The starter is **deliberately wrong but compiles**. Run the tests first and look at which cases pass and which fail; the pattern tells you exactly what the bug is.
