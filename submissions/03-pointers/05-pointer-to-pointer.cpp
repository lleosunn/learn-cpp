#include <cstddef>
#include <iostream>

// Search arr[0..n) for target. Return whether it was found.
// If out is not null, set *out to a pointer to the first match, or to nullptr if none.
bool find_first(int* arr, std::size_t n, int target, int** out) {
    // TODO: reset *out, scan, write the match through out
    if (out) *out = nullptr; // if out points to something, set the pointer that out points to to nullptr
    for (std::size_t i = 0; i < n; ++i) { // for all the indices in the array
        if (arr[i] == target) { // if the integer at the current index is equal to the target
            if (out) *out = arr + i; // 
            return true;
        }
    }
    return false;
}