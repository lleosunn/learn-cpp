# Digit Sum

Welcome to C++. Before pointers and memory, you need two things: the compile-run loop, and the idea that **every value has a fixed type**.

## The lesson

In Python you would write:

```python
def digit_sum(n):
    ...
```

In C++ the same function announces its types up front:

```cpp
int digit_sum(int n) {
    // ...
    return 0;
}
```

- The `int` before the name is the **return type**. This function must hand back an `int`, on every path.
- `int n` says the parameter is an `int`. Pass a string and the program does not compile.
- Types are checked by the **compiler** before anything runs. A whole class of Python runtime errors becomes compile errors here. You will see a lot of them. Read them top to bottom; the first one is usually the real one.

Two operators do all the work in this problem:

| C++ | meaning | example |
|---|---|---|
| `n % 10` | remainder | `1234 % 10 == 4` |
| `n / 10` | **integer** division, truncates | `1234 / 10 == 123` |

Integer division truncating toward zero is what makes a `while (n > 0)` loop terminate: `1234 → 123 → 12 → 1 → 0`.

## Coming from Python

- `%` and `/` on negatives differ. Python floors (`-99 % 10 == 1`); C++ truncates toward zero (`-99 % 10 == -9`). Take the absolute value first.
- There is no `**`, and no arbitrary-precision integers. An `int` is 32 bits here, roughly ±2.1 billion. Chapter 1 problem 4 is about what happens past that.
- `while` needs parentheses around its condition and (by convention) braces around its body. Every statement ends with `;`.
- `n /= 10;` works like Python's `n //= 10`.

## Your task

Return the sum of the decimal digits of `|n|`.

```cpp
int digit_sum(int n);
```

| call | result |
|---|---|
| `digit_sum(1234)` | `10` |
| `digit_sum(-99)` | `18` |
| `digit_sum(0)` | `0` |

Click **Run tests** (⌘↵) whenever you want feedback. The starter compiles and returns `0`, so you will see exactly which cases fail. The **Tests** tab shows how your function gets called.
