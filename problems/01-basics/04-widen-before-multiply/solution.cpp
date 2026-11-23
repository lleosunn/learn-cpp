long long product(int a, int b) {
    // Widen one operand BEFORE multiplying; the other is promoted to match,
    // so the multiplication itself is done in 64 bits.
    // static_cast<long long>(a * b) would still multiply in int and overflow.
    return static_cast<long long>(a) * b;
}
