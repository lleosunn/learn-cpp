# Swap Two Ints

In Python every argument is a reference to an object. In C++ the default is the opposite: every argument is a **copy**. This problem is about the one character that changes that.

## The lesson

```cpp
void bump(int n) { n += 1; }

int x = 5;
bump(x);
// x is still 5
```

`n` is a brand-new `int` living in `bump`'s stack frame, initialized with a copy of `x`'s value. Modifying `n` modifies the copy. When `bump` returns, `n` is gone and `x` never noticed.

To let a function touch the caller's variable, declare the parameter as a **reference** with `&`:

```cpp
void bump(int& n) { n += 1; }

int x = 5;
bump(x);
// x is now 6
```

`int& n` means "`n` is another name for whatever `int` the caller passed." It is not a copy and not a pointer you have to dereference; you use `n` exactly like an `int`, and every read and write goes to `x`. Inside `bump`, `&n == &x`: same address.

Rules that matter:

- A reference must be initialized when it is created and can never be re-seated. `n = 7` writes `7` into `x`; it does not make `n` refer to a `7` somewhere else.
- A non-const `int&` only binds to an actual variable (an *lvalue*). `bump(5)` and `bump(x + 1)` do not compile, because there is nothing for the caller to see modified.
- `void` is the return type for "returns nothing."

Why the copy default is a feature: a function that takes `int n` *cannot* change your variables, and both you and the compiler know it from the signature alone. The price is that when you do want to modify the caller's state, you must say `&` explicitly. Forgetting it does not produce an error; it produces a function that silently does nothing, which is exactly what the starter does.

## Coming from Python

- Python's `a, b = b, a` rebinds two *names*. There is no C++ equivalent that works on names; `swap_ints` has to write new values *into* the caller's two objects.
- "Python passes by reference" is the wrong model. Python passes object references by value: mutating a list inside a function is visible outside, but `n += 1` on an int is not. In C++, nothing is visible outside unless the parameter is a reference (or a pointer, next chapter).
- Python functions that "return nothing" return `None`. A C++ `void` function returns nothing at all; `int y = swap_ints(a, b);` is a compile error.

## Your task

Exchange the values of the two variables the caller passes.

```cpp
void swap_ints(int& a, int& b);
```

| before | call | after |
|---|---|---|
| `a = 1, b = 2` | `swap_ints(a, b)` | `a = 2, b = 1` |
| `a = -3, b = 7` | `swap_ints(a, b)` | `a = 7, b = -3` |
| `a = 5, b = 5` | `swap_ints(a, b)` | `a = 5, b = 5` |
| `x = 9` | `swap_ints(x, x)` | `x = 9` |

The starter is deliberately wrong but compiles: it swaps two copies and the caller sees nothing. Run it first and look at which cases fail, then fix the signature. (Real code would call `std::swap(a, b)` from `<utility>`; here, write the three lines yourself.)
