# Swap Two Pointers

A second lap on `int**`, the idea from problem 5, in its smallest possible form.

## The lesson

In problem 2, swapping your local pointers was the bug: the caller's ints had to change, and re-pointing your own copies did nothing. This time the caller's **pointers** are what must change, and the ints must stay exactly where they are.

```text
before:   p: [ &x ]    q: [ &y ]        x: [ 1 ]    y: [ 2 ]
after:    p: [ &y ]    q: [ &x ]        x: [ 1 ]    y: [ 2 ]    (the ints never moved)
```

The rule has not changed since Chapter 2: to change a caller's variable, you need its address. The caller's variables are `int*`, so their addresses are `int**`:

```cpp
int x = 1, y = 2;
int* p = &x;
int* q = &y;
swap_pointers(&p, &q);   // &p is an int**
// now p == &y and q == &x
```

Inside the function, each star strips one level:

| expression | type | what it is |
|---|---|---|
| `a` | `int**` | your local copy of the address of the caller's `p` |
| `*a` | `int*` | the caller's `p` itself |
| `**a` | `int` | the int that `p` points at (`x`) |

Assigning to `a` changes nothing the caller can see. Assigning to `*a` re-points the caller's `p`. Assigning to `**a` overwrites `x`. Three different boxes, and picking the right one is the whole problem.

A swap needs a temporary of the type being swapped. You are swapping two `int*`, so the temporary is an `int*`.

Since the job never looks at the ints, it never needs `**`. That is not just tidiness: `p` or `q` may hold `nullptr`, and that is fine. Copying a null pointer from one variable to another is an ordinary assignment. Only *dereferencing* it is undefined behavior.

The starter is deliberately wrong in the opposite direction from problem 2: it swaps `**a` and `**b`. It compiles, and afterward `*p` and `*q` even look swapped. But `x` and `y` have been overwritten, `p` and `q` still point where they did, and a null `q` crashes it. Run it first.

## Coming from Python

- `p, q = q, p` rebinds two names, which is exactly this operation. What Python cannot do is let a *function* rebind its caller's names. `int**` is how C does it.
- Rebinding does not touch the objects: after `p, q = q, p` the two objects are unchanged, just as `x` and `y` are here.
- `std::swap(*a, *b)` is the one-liner. It works for any type, including pointers.
- You will see this shape again in Chapter 7: moving ownership is swapping or stealing pointers while the data stays put.

## Your task

Swap the caller's two pointers. Do not change any int. `a` and `b` are never null, but the pointers they point at may be.

```cpp
void swap_pointers(int** a, int** b);
```

| call | afterward |
|---|---|
| `swap_pointers(&p, &q)` with `p = &x`, `q = &y` | `p == &y`, `q == &x`; `x` and `y` unchanged |
| `swap_pointers(&p, &p)` | `p` unchanged |
| `swap_pointers(&first, &last)` with `first = arr`, `last = arr + 2` | `first == arr + 2`, `last == arr`; array unchanged |
| `swap_pointers(&r, &n)` with `r = &x`, `n = nullptr` | `r == nullptr`, `n == &x`, no crash |
