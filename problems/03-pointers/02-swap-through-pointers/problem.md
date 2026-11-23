# Swap via Pointers

You have received an `int*` and read through it. Now you make one with `&`, write through it, and see exactly how it differs from a reference.

## The lesson

Chapter 2's swap took `int&`. The pointer version takes `int*`, and the caller has to spell out the address:

```cpp
void swap_ints(int& a, int& b);    // Chapter 2: caller writes swap_ints(x, y);
bool swap_through(int* a, int* b);  // now: caller writes swap_through(&x, &y);
```

Inside the function, `a` and `b` are **local copies of two addresses**. Memory looks like this:

```text
caller's frame             swap_through's frame
x: [ 1 ]  <--------------  a: [ &x ]
y: [ 2 ]  <--------------  b: [ &y ]
```

Everything hinges on which box you touch:

- `a = b;` overwrites the local box `a` with a different address. `x` and `y` do not move. The caller's variables are untouched and the function has done nothing useful.
- `*a = *b;` reads the int at `b` and writes it into the int at `a`. That changes `x`.

The starter makes the first mistake. It swaps the arrows, not the values, and it compiles cleanly because reassigning a pointer is perfectly legal. This is the same bug shape as mutating a copy of a by-value parameter, one level of indirection up. The compiler cannot tell "swap the ints" from "shuffle my own local pointers", so this one is on you.

Pointer vs reference, side by side:

| | `int&` | `int*` |
|---|---|---|
| can be null | no | yes (`nullptr`) |
| can be re-pointed later | no | yes (`p = &other;`) |
| syntax to reach the value | `a` | `*a` |
| caller writes | `f(x)` | `f(&x)` |

Use a reference when "nothing" is not a valid input. Use a pointer when it is, and then check for it. Here a null on either side means "nothing to swap": return `false` and leave both untouched.

Why `swap_through(&x, &x)` is in the tests: both pointers point at the same int. `int t = *a; *a = *b; *b = t;` still works, because reading through either one gives the same value. Aliasing like this is normal in C++, and your code should survive it.

## Coming from Python

- There is no Python equivalent. The closest description is "an explicit reference that is also allowed to be `None`".
- `&x` in an expression means "address of x". It is not the `&` that declares a reference parameter (`int& a`): same character, different position, different meaning.
- Python's `a, b = b, a` rebinds two names. Here you swap the contents of two memory locations the caller owns.
- `if (!a || !b)` works because a pointer converts to `bool`: null is `false`, anything else is `true`.

## Your task

Swap the two ints `a` and `b` point at and return `true`. If either pointer is null, change nothing and return `false`.

```cpp
bool swap_through(int* a, int* b);
```

| call | result |
|---|---|
| `swap_through(&x, &y)` with `x = 1, y = 2` | `true`; afterward `x == 2`, `y == 1` |
| `swap_through(&x, &x)` | `true`; `x` unchanged |
| `swap_through(nullptr, &y)` | `false`; `y` unchanged |
| `swap_through(nullptr, nullptr)` | `false` |

The starter compiles and returns `true`, but it only rearranges its own local pointers. Run it first and watch `x` and `y` stay put.
