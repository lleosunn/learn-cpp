# Generic and Compile-Time Functions

Python functions are generic for free: `max_of(v)` works on any list whose elements support `<`. C++ templates give you the same thing, checked at compile time. `constexpr` goes one step further: the compiler runs the function for you.

## The lesson

**Function templates.** Write the function once with a placeholder type:

```cpp
template <typename T>
T max_of(const std::vector<T>& v) {
    T best = v[0];
    for (const T& x : v)
        if (best < x) best = x;
    return best;
}
```

Call `max_of(ints)` with a `std::vector<int>` and the compiler *deduces* `T = int` and stamps out an `int` version. Call it with `std::vector<std::string>` and you get a second, separate function for strings. Nothing is checked until that stamping ("instantiation") happens, so a template's requirements are implicit: the body above uses `<` and copying, so any `T` with those works. Pass a type without `<` and the error points at the line *inside the template* that used it, with a note naming your call site. Read the note; that is where the fix goes.

This is duck typing at compile time, with no runtime cost. Two consequences:

- The compiler needs the entire template body wherever it is instantiated, so templates live in headers (here, in `solution.cpp`, which the tests `#include`). A template in a `.cpp` file nobody includes is invisible.
- Deduction needs a real argument. `max_of({1, 2})` fails to compile: a braced list has no type, so `T` cannot be deduced. The tests pass named vectors for exactly this reason.

**Callables as template parameters.** `template <typename T, typename Pred>` lets `filter(v, keep)` accept a lambda, a plain function, or anything else callable, with no `std::function` overhead: `Pred` becomes the callable's exact type and `keep(x)` is a direct call.

**`constexpr` functions** can run at compile time when given constant arguments:

```cpp
constexpr int square(int x) { return x * x; }
static_assert(square(12) == 144, "checked by the compiler");
int table[square(3)];   // 9 is a constant expression, so this is a legal array size
```

Since C++14 a `constexpr` function body may contain loops, locals, and `if`; it may not do I/O, allocate, or touch globals. Called with runtime values it is an ordinary function. The keyword grants permission, it does not force anything. Without it, `static_assert(ipow(3, 4) == 81)` is a compile error: "non-constexpr function 'ipow' cannot be used in a constant expression".

Why bother? Table sizes, bit masks, and unit conversions get computed once by the compiler instead of on every run, and `static_assert` turns "I believe this is 81" into something the build verifies.

## Coming from Python

- Duck typing checks at the call, at runtime, one object at a time. Templates check at instantiation, at compile time, for the whole type. Both are structural: no base class or interface declaration required.
- `key=` and `filter(pred, xs)` accept any callable. `Pred` is the same idea with a type parameter.
- Python has no compile-time evaluation; `constexpr` is closest to "a pure function the compiler may precompute".
- `std::pair<T, T>` is a two-tuple with a fixed type per slot. Unpack it with `.first` / `.second` or a structured binding: `auto [lo, hi] = min_max(v);`.

## Your task

```cpp
template <typename T> T max_of(const std::vector<T>& v);                 // v non-empty
template <typename T> std::pair<T, T> min_max(const std::vector<T>& v);  // v non-empty; (smallest, largest)
template <typename T, typename Pred>
std::vector<T> filter(const std::vector<T>& v, Pred keep);               // elements x with keep(x) true, order kept
constexpr long long ipow(long long base, int exp);                       // base to the power exp, exp >= 0
```

Compare with `<` only, so every type that has `operator<` works (`std::max_element` and `std::minmax_element` from `<algorithm>` already do). The tests contain `static_assert(ipow(3, 4) == 81)`, which refuses to compile until `ipow` is a correct `constexpr` function, so the whole test file stays red until then. Do `ipow` first if you want early feedback on the others.

| call | result |
|---|---|
| `max_of(std::vector<int>{3, 9, 2})` | `9` |
| `max_of(std::vector<std::string>{"pear", "apple"})` | `"pear"` |
| `min_max(std::vector<int>{3, 1, 2})` | `(1, 3)` |
| `filter(std::vector<int>{1, 2, 3, 4}, is_even)` | `{2, 4}` |
| `ipow(2, 10)` | `1024` |
| `ipow(7, 0)` | `1` |
