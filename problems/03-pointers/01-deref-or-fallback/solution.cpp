int value_or(const int* p, int fallback) {
    // Test the pointer, then read through it. Never *p on a null pointer.
    return p ? *p : fallback;
}
