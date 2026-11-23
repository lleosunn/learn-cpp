// Same name, different parameter types: the compiler picks by the argument's static type.
// Defaults fill from the right: clamp_value(150) means clamp_value(150, 0, 100).
int clamp_value(int x, int lo = 0, int hi = 100) {
    return x < lo ? lo : (x > hi ? hi : x);
}

double clamp_value(double x, double lo = 0.0, double hi = 1.0) {
    return x < lo ? lo : (x > hi ? hi : x);
}
