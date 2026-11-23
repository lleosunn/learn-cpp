#include <cstddef>

bool find_first(int* arr, std::size_t n, int target, int** out) {
    if (out) *out = nullptr;          // reset first: a miss must not leave a stale pointer
    for (std::size_t i = 0; i < n; ++i) {
        if (arr[i] == target) {
            if (out) *out = arr + i;  // *out is the caller's int*; this is the write-through
            return true;
        }
    }
    return false;
}
