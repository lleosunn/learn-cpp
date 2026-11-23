#include <cstddef>
#include "tracked.h"

Tracked* make_tracked_array(std::size_t n, int first_value) {
    Tracked* p = new Tracked[n];  // n default-constructed objects, contiguous; legal for n == 0
    for (std::size_t i = 0; i < n; ++i) {
        p[i].value = first_value + static_cast<int>(i);
    }
    return p;
}

void free_tracked_array(Tracked* p) {
    delete[] p;  // array form: runs every destructor, then frees the block
}
