#include <algorithm>
#include <vector>

// Set lo/hi to the smallest/largest element of v and return true.
// If v is empty, return false and leave lo and hi exactly as they were.
bool min_max(const std::vector<int>& v, int& lo, int& hi) {
    // TODO
    if (v.empty()) return false;
    lo = hi = v[0];
    for (int n : v) {
        lo = std::min(n, lo);
        hi = std::max(n, hi);
    }
    return true;
}
