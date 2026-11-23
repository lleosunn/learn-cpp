# Fraction with Operators

Python lets you make `a + b` and `a == b` work for your own types by defining `__add__` and `__eq__`. C++ has the same idea, called operator overloading, and this problem uses it to build a rational number that is always in lowest terms.

## The lesson

**Invariants live in the constructor.** A `Fraction` promises three things: the denominator is positive, numerator and denominator share no common factor, and zero is stored as `0/1`. If the constructor guarantees all three, every other function can rely on them. Equality becomes a plain comparison of two fields instead of cross-multiplication, and printing is always canonical. This is the single most useful habit in class design: establish the invariant once, at construction, and never let an object exist that violates it.

```cpp
Fraction(long long num = 0, long long den = 1) : num_(num), den_(den) {
    if (den_ < 0) { num_ = -num_; den_ = -den_; }   // sign lives in the numerator
    long long g = std::gcd(num_, den_);             // <numeric>, C++17
    num_ /= g;
    den_ /= g;
}
```

`std::gcd` is defined on absolute values, so a negative numerator is fine, and `std::gcd(0, 5) == 5` means `0/5` reduces to `0/1` with no special case. The two default arguments make this one constructor serve as the no-argument constructor (`Fraction{}` is `0/1`) and the whole-number constructor (`Fraction(3)` is `3/1`).

**Operators are functions with funny names.** `a + b` is a call to `operator+(a, b)`; `a == b` is `operator==(a, b)`. You can write these as member functions, but for symmetric binary operators the idiom is a non-member function taking both operands as `const T&`:

```cpp
Fraction operator+(const Fraction& a, const Fraction& b);
bool operator==(const Fraction& a, const Fraction& b);
```

Non-member means neither operand is special, and the operators only use the public interface (`num()`, `den()`), which is a nice sign that the class exposes enough.

**Streaming.** `std::cout << f` is `operator<<(std::cout, f)`. The left operand is a stream you do not own, so `<<` *must* be a non-member:

```cpp
std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    return os << f.num() << '/' << f.den();
}
```

Why return the stream? `os << a << b` parses as `(os << a) << b`. The inner call has to produce something that `<< b` can be applied to, and that something is the stream itself. Return `void` instead and chaining is a compile error. Forget the `return` statement and it is worse: the code still compiles (clang only warns, `-Wreturn-type`) and is undefined behavior at run time; UBSan aborts with `execution reached the end of a value-returning function without returning a value`. The test streams into a `std::ostringstream`, which collects output in a string you can inspect with `.str()`; the same operator works for `std::cout` and for files.

## Coming from Python

- `__add__` is `operator+`, `__eq__` is `operator==`, `__repr__`/`__str__` is `operator<<`. There is no `__radd__`: a non-member `operator+(const Fraction&, const Fraction&)` is already symmetric.
- Python derives `!=` from `__eq__`. C++17 does not; you would write `operator!=` by hand (C++20 generates it). Not needed here.
- `fractions.Fraction` normalizes exactly like this, which is why `Fraction(2, 4) == Fraction(1, 2)` is `True` there too.
- `long long` overflows past about 9.2 × 10^18. Python's ints do not. The tests stay small.

## Your task

Implement the constructor so every `Fraction` is normalized (positive denominator, lowest terms, zero as `0/1`), then the three non-member operators. `den` is never 0.

```cpp
class Fraction {
public:
    Fraction(long long num = 0, long long den = 1);
    long long num() const;
    long long den() const;
};
Fraction operator+(const Fraction& a, const Fraction& b);
bool operator==(const Fraction& a, const Fraction& b);
std::ostream& operator<<(std::ostream& os, const Fraction& f);   // prints "n/d"
```

| expression | result |
|---|---|
| `Fraction(1, 2) + Fraction(1, 3)` | `5/6` |
| `Fraction(2, 4)` | `1/2` |
| `Fraction(3, -6)` | `-1/2` |
| `Fraction(0, 5)` | `0/1` |
| `Fraction(1, 2) == Fraction(2, 4)` | `true` |
| `oss << Fraction(-1, 2) << " " << Fraction(3)` | `oss.str()` is `"-1/2 3/1"` |
| `Fraction(1, 3) + Fraction(2, 3)` | `1/1` |
