#include <cstddef>

// Sum of arr[0..n). Must not overflow for any two ints.
long long sum_array(const int* arr, std::size_t n) {
    // TODO: loop i from 0 up to (not including) n and accumulate
    long long total = 0;
    for (std::size_t i = 0; i < n; ++i) {
        total += arr[i];
    }
    return total;
}

// Pointer to the first maximum element of arr[0..n), or nullptr if n == 0.
int* max_element_ptr(int* arr, std::size_t n) {
    // TODO: keep a pointer to the best element seen so far
    if (n == 0) return nullptr;
    int* best_ptr = &arr[0];
    for (std::size_t i = 0; i < n; ++i) {
        if (*best_ptr < arr[i]) best_ptr = &arr[i];
    }
    return best_ptr;
}
