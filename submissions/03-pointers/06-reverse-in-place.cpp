#include <cstddef>

// Reverse arr[0..n) in place.
void reverse_array(int* arr, std::size_t n) {
    // TODO: one pointer at the front, one starting at arr + n; swap and step inward
    int* lo = arr;
    int* hi = arr + n;
    while (lo < hi) {
        --hi;
        int temp = *hi;
        *hi = *lo;
        *lo = temp;
        ++lo;
        
    }
    
}
