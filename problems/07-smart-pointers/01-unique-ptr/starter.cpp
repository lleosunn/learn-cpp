#include <memory>

// A new Tracked holding v, owned by the returned unique_ptr.
std::unique_ptr<Tracked> make_tracked(int v) {
    // TODO: std::make_unique
    return nullptr;
}

// p->value, or -1 if p is empty.
int value_of(const std::unique_ptr<Tracked>& p) {
    // TODO
    return -1;
}

// Make p own a new Tracked(v). The object it owned before must be destroyed.
void replace_with(std::unique_ptr<Tracked>& p, int v) {
    // TODO
}
