#include <algorithm>
#include <cstddef>
#include "tracked.h"

// Return a fresh heap array of new_n Tracked holding the first min(old_n, new_n)
// values of old, then release old. old may be nullptr when old_n == 0.
//
// This starter compiles and is WRONG on purpose. Run the tests first and read
// the AddressSanitizer report before you change anything.
Tracked* grow(Tracked* old, std::size_t old_n, std::size_t new_n) {
    Tracked* p = new Tracked[new_n];
    for (std::size_t i = 0; i < std::min(old_n, new_n); ++i) {
        p[i].value = old[i].value;
    }
    delete[] old;
    return p;
}
