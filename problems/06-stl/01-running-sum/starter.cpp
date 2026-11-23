#include <cstddef>
#include <vector>

// Return the prefix sums of v: result[i] == v[0] + ... + v[i].
std::vector<int> running_sum(const std::vector<int>& v) {
    // TODO: build a new vector with push_back (reserve first)
    return {};
}

// If i is a valid index, store v[i] in out and return true.
// Otherwise return false and leave out untouched.
bool get_at(const std::vector<int>& v, std::size_t i, int& out) {
    // TODO: this reads past the end when i >= v.size(). Run it and see.
    out = v[i];
    return true;
}
