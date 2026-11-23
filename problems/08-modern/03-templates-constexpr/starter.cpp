#include <algorithm>
#include <string>
#include <utility>
#include <vector>

// Largest element of a non-empty vector. Must work for any T with operator<.
template <typename T>
T max_of(const std::vector<T>& v) {
    // TODO
    return v[0];
}

// (smallest, largest) of a non-empty vector.
template <typename T>
std::pair<T, T> min_max(const std::vector<T>& v) {
    // TODO
    return {v[0], v[0]};
}

// The elements x of v, in order, for which keep(x) is true.
template <typename T, typename Pred>
std::vector<T> filter(const std::vector<T>& v, Pred keep) {
    // TODO
    return {};
}

// base to the power exp, exp >= 0. Must be usable inside a static_assert.
long long ipow(long long base, int exp) {
    // TODO: make this constexpr and compute the power with a loop
    return 0;
}
