// Swap the caller's two pointers. The ints they point at must not change.
// a and b are never null; *a and *b may be.
// This starter is deliberately wrong: it swaps the ints instead of the pointers. Run it first.
void swap_pointers(int** a, int** b) {
    // TODO: this goes one level too deep. Swap *a and *b, not **a and **b.
    int *t = *a;
    *a = *b;
    *b = t;
}
