bool swap_through(int* a, int* b) {
    if (!a || !b) return false;  // a null pointer converts to false
    int t = *a;                  // read the int a points at (the caller's x)
    *a = *b;                     // write through a: x now holds y's value
    *b = t;                      // write through b
    return true;                 // (std::swap(*a, *b) is the one-liner once you trust it)
}
