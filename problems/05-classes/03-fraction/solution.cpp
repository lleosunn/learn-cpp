#include <numeric>
#include <ostream>

class Fraction {
public:
    // The constructor establishes the invariant: den_ > 0, lowest terms, zero is 0/1.
    Fraction(long long num = 0, long long den = 1) : num_(num), den_(den) {
        if (den_ < 0) { num_ = -num_; den_ = -den_; }   // keep the sign in the numerator
        long long g = std::gcd(num_, den_);             // gcd of absolute values; gcd(0, d) == d
        num_ /= g;
        den_ /= g;
    }

    long long num() const { return num_; }
    long long den() const { return den_; }

private:
    long long num_;
    long long den_;
};

// Non-member: symmetric, and only needs the public interface.
Fraction operator+(const Fraction& a, const Fraction& b) {
    return Fraction(a.num() * b.den() + b.num() * a.den(), a.den() * b.den());
}

// Both sides are normalized, so field-wise comparison is exact.
bool operator==(const Fraction& a, const Fraction& b) {
    return a.num() == b.num() && a.den() == b.den();
}

// Returns the stream so that  os << a << b  chains.
std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    return os << f.num() << '/' << f.den();
}
