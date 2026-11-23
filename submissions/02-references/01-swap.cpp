// Exchange the values of the caller's two variables.
// This version compiles and runs, but the caller never sees a change. Why?
void swap_ints(int& a, int& b) {
    int t = a;
    a = b;
    b = t;
}
