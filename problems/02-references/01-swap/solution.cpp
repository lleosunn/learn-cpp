// a and b are references: other names for the caller's variables, not copies.
void swap_ints(int& a, int& b) {
    int t = a;  // t is a plain int: a copy of the value, which is what we want here
    a = b;      // writes into the caller's first variable
    b = t;      // writes into the caller's second variable
}
