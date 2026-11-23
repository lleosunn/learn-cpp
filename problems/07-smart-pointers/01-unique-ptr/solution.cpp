#include <memory>

std::unique_ptr<Tracked> make_tracked(int v) {
    return std::make_unique<Tracked>(v);   // allocate + wrap in one expression; no raw new
}

int value_of(const std::unique_ptr<Tracked>& p) {
    return p ? p->value : -1;              // unique_ptr tests like a pointer
}

void replace_with(std::unique_ptr<Tracked>& p, int v) {
    p = std::make_unique<Tracked>(v);      // assignment deletes the old object, then takes the new one
}
