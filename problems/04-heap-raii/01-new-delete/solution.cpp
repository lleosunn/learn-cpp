#include "tracked.h"

Tracked* make_tracked(int v) {
    return new Tracked(v);  // heap object: survives the return; the caller must delete it
}

void destroy_tracked(Tracked* p) {
    delete p;  // runs ~Tracked and frees the memory; delete nullptr is a no-op
}
