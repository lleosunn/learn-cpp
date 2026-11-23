// Return a * b exactly, as a 64-bit integer.
long long product(int a, int b) {
    // TODO: this compiles. Run the tests and read the UBSan report.
    c = static_cast<long long>(a);
    d = static_cast<long long>(b);
    return c*d;
}
