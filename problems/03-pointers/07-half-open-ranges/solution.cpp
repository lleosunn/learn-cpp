#include <cstddef>

const int* find_in_range(const int* first, const int* last, int target) {
    for (const int* p = first; p != last; ++p) {  // the pointer is the loop variable
        if (*p == target) return p;
    }
    return last;  // "not found" is the end of the range, never nullptr
}

std::size_t count_in_range(const int* first, const int* last, int target) {
    std::size_t count = 0;
    for (const int* p = first; p != last; ++p) {
        if (*p == target) ++count;
    }
    return count;
}
