#include <algorithm>
#include <cstddef>
#include "tracked.h"

Tracked* grow(Tracked* old, std::size_t old_n, std::size_t new_n) {
    Tracked* p = new Tracked[new_n];           // 1. allocate the new block
    std::size_t keep = std::min(old_n, new_n);
    for (std::size_t i = 0; i < keep; ++i) {   // 2. copy the survivors while old is still alive
        p[i].value = old[i].value;
    }
    delete[] old;                              // 3. only now release the old block
    return p;
}
