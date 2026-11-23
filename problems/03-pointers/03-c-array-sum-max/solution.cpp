#include <cstddef>

long long sum_array(const int* arr, std::size_t n) {
    long long total = 0;
    for (std::size_t i = 0; i < n; ++i) {
        total += arr[i];  // arr[i] is *(arr + i); the int widens to long long before the add
    }
    return total;
}

int* max_element_ptr(int* arr, std::size_t n) {
    if (n == 0) return nullptr;
    int* best = arr;  // &arr[0]
    for (std::size_t i = 1; i < n; ++i) {
        if (arr[i] > *best) best = arr + i;  // strict > keeps the first of equal maxima
    }
    return best;
}
