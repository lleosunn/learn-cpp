// Clamp x into [lo, hi]. Two overloads are needed: one for int, one for double.
// Defaults: int version [0, 100], double version [0.0, 1.0].
# include <algorithm>

int clamp_value(int x, int lo = 0, int hi = 100) {
    return std::clamp(x, lo, hi);
}

// TODO: double overload
double clamp_value(double x, double lo = 0.0, double hi = 1.0) {
    return std::clamp(x, lo, hi);
}