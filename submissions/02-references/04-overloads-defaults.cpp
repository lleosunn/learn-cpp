// Clamp x into [lo, hi]. Two overloads are needed: one for int, one for double.
// Defaults: int version [0, 100], double version [0.0, 1.0].
int clamp_value(int x, int lo = 0, int hi = 100) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

// TODO: double overload
double clamp_value(double x, double lo = 0.0, double hi = 1.0) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}