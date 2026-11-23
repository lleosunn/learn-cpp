#include <numeric>
#include <ostream>

class Fraction {
public:
    // TODO: normalize so den_ > 0 and gcd(num_, den_) == 1 (zero is 0/1)
    Fraction(long long num = 0, long long den = 1) : num_(num), den_(den) {}

    long long num() const { return num_; }
    long long den() const { return den_; }

private:
    long long num_;
    long long den_;
};

// TODO: (a*d + c*b) / (b*d), then let the constructor normalize
Fraction operator+(const Fraction& a, const Fraction& b) {
    return Fraction{};
}

// TODO: both fractions are normalized, so compare the parts
bool operator==(const Fraction& a, const Fraction& b) {
    return false;
}

// TODO: print "n/d" and return the stream so << chains
std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    return os;
}
