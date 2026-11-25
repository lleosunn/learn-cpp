#include "tracked.h"

// Return a heap-allocated Tracked whose value is v. The caller owns it.
Tracked* make_tracked(int v) {
    // TODO: allocate on the heap so the object outlives this function
    return new Tracked(v);
}

// Destroy an object returned by make_tracked. Must be safe with nullptr.
void destroy_tracked(Tracked* p) {
    // TODO: repay the debt
    delete p;
}
