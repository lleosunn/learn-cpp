#include <cstddef>
#include "tracked.h"

// Return a heap array of n Tracked where element i has value first_value + i.
// n may be 0. The caller owns the array.
Tracked* make_tracked_array(std::size_t n, int first_value) {
    // TODO: new[] the block, then fill in the values
    Tracked* out = new Tracked[n];
    for (std::size_t i = 0; i < n; ++i) {
        out[i].value = static_cast<int>(i) + first_value;
    }
    return out;
}

// Release an array returned by make_tracked_array.
void free_tracked_array(Tracked* p) {
    // TODO: the array form of delete
    delete[] p;
}
