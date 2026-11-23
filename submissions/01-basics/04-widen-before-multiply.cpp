// Return a * b exactly, as a 64-bit integer.
long long product(int a, int b) {
    // TODO: this compiles. Run the tests and read the UBSan report.
    a = static_cast<long long>(a);
    b = static_cast<long long>(b);
    return a*b;
}
