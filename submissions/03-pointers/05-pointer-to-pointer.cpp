#include <cstddef>
#include <iostream>

// Search arr[0..n) for target. Return whether it was found.
// If out is not null, set *out to a pointer to the first match, or to nullptr if none.
bool find_first(int* arr, std::size_t n, int target, int** out) {
    // TODO: reset *out, scan, write the match through out
    if (out) *out = nullptr;
    for (std::size_t i = 0; i < n; ++i) {
        if (arr[i] == target) {
            if (out) *out = arr + i;
            return true;
        }
    }
    return false;
}