void swap_pointers(int** a, int** b) {
    int* t = *a;  // *a is the caller's pointer; the temporary holds an address
    *a = *b;      // re-point the caller's first pointer
    *b = t;       // never **: the ints are not read, so a null *a or *b is fine
}
