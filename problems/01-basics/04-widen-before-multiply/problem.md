# Product Without Overflow

Python integers grow as large as they need to. C++ integers have a fixed number of bits, and running a signed one past its edge is not a wrap-around, it is undefined behavior.

## The lesson

Every integer type has a fixed width. On this machine, and on almost every 64-bit platform you will meet:

| type | bits | range |
|---|---|---|
| `int` | 32 | about ±2.1 × 10⁹ |
| `long long` | 64 | about ±9.2 × 10¹⁸ |
| `long` | 64 here, 32 on Windows | platform-dependent; avoid in portable code |

The standard only guarantees minimums (`int` at least 16 bits, `long long` at least 64), so when the width matters, say it: `<cstdint>` gives `std::int32_t`, `std::int64_t`, `std::uint8_t` and friends. You can ask the compiler with `sizeof(int)` (in bytes) and `std::numeric_limits<int>::max()` from `<limits>`; `<climits>` provides the older `INT_MAX` and `INT_MIN` macros.

**Signed overflow is undefined behavior.** Not "wraps around", not "throws". The standard says a program that does it has no defined meaning, and the compiler is allowed to assume it never happens. In practice the optimizer may delete your overflow check, turn a bounded loop infinite, or give a different answer at `-O2` than at `-O0`. (Unsigned types are different: they wrap modulo 2ⁿ by definition. That is well-defined, and usually still a bug.)

This course builds with UBSan, which inserts a runtime check and aborts with a report the moment a signed overflow happens. Production builds do not; they just go wrong quietly. Treat the report as a gift.

**Arithmetic happens in the type of the operands.** Same rule as the integer-division problem, same trap:

```cpp
int a = 100000, b = 100000;
long long r1 = a * b;                            // int * int overflows, THEN widens the wreckage
long long r2 = static_cast<long long>(a * b);    // identical: the cast applies to the result
long long r3 = static_cast<long long>(a) * b;    // a is widened first, so b is promoted: 64-bit multiply
long long r4 = 1LL * a * b;                      // same idea: the LL literal drags the others up
```

Only `r3` and `r4` are correct. The destination type of an assignment or a `return` never changes how the right-hand side is evaluated.

The habit to build: before any `+ - *` on values that might be large, ask "in what type is this computed, and does the true result fit?" If the answer is no, widen an operand first.

## Coming from Python

- Python `int` is arbitrary precision; `10**20` just works. In C++ you choose a width up front and are responsible for staying inside it.
- Python has no undefined behavior. The idea that the compiler may *assume* something and miscompile you when it is false is new. UB is not an exception you can catch; it is a hole in the contract.
- Literal suffixes: an unsuffixed literal that does not fit in `int` is automatically given a wider type, but `1LL` forces a small literal to be `long long`.
- `static_cast<long long>(x)` is the C++ `int(x)`. Widening is always lossless; narrowing (the other direction) is the one to watch.

## Your task

Return the exact product of two `int` values. The true result always fits in a `long long`.

```cpp
long long product(int a, int b);
```

| call | result |
|---|---|
| `product(3, 4)` | `12` |
| `product(100000, 100000)` | `10000000000` |
| `product(-100000, 100000)` | `-10000000000` |
| `product(INT_MAX, 1)` | `2147483647` |
| `product(INT_MIN, -1)` | `2147483648` |

The starter is **deliberately wrong but compiles**. Run it first: the first case passes, then UBSan stops the program with a `signed-integer-overflow` report. Read it; it names the line and the two values being multiplied.
