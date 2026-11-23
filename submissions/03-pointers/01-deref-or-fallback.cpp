// Return the int p points at, or fallback if p is null.
// This starter is deliberately missing the null check. Run it first and read the report.
int value_or(const int* p, int fallback) {
    // TODO: check p before dereferencing it
    if (p == nullptr) return fallback;
    return *p;
}
