#include <cstddef>

// Pointer to the first element of [first, last) equal to target, or last if there is none.
const int* find_in_range(const int* first, const int* last, int target) {
    // TODO: walk a pointer p from first while p != last
    while (first != last) {
        if (*first == target) return first;
        ++first;
    }
    return last;
}

// How many elements of [first, last) equal target.
std::size_t count_in_range(const int* first, const int* last, int target) {
    // TODO
    int count = 0;
    while (first != last) {
        if (*first == target) ++count;
        ++first;
    }
    return count;
}
