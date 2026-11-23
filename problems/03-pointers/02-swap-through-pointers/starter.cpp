// Swap the ints a and b point at and return true.
// If either pointer is null, change nothing and return false.
// This starter is deliberately wrong: it compiles, returns true, and swaps nothing. Run it first.
bool swap_through(int* a, int* b) {
    // TODO: this swaps the local pointers, not the ints they point at. Also handle null.
    int* t = a;
    a = b;
    b = t;
    return true;
}
