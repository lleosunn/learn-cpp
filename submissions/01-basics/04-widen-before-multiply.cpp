// Return a * b exactly, as a 64-bit integer.
long long product(int a, int b) {
    // TODO: this compiles. Run the tests and read the UBSan report.
    long long c = static_cast<long long>(a);
    long long d = static_cast<long long>(b);
    return c*d;
}
