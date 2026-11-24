#include <cstddef>

void reverse_array(int* arr, std::size_t n) {
    int* lo = arr;
    int* hi = arr + n;  // one past the end: legal to hold, never to dereference
    while (lo < hi) {
        --hi;           // step back first, so hi is on a real element
        int t = *lo;
        *lo = *hi;
        *hi = t;
        ++lo;
    }
}
