#include <algorithm>
#include <vector>

bool min_max(const std::vector<int>& v, int& lo, int& hi) {
    if (v.empty()) return false;  // precondition first: outputs stay untouched
    lo = hi = v[0];               // writes go straight into the caller's variables
    for (std::size_t i = 1; i < v.size(); ++i) {
        lo = std::min(lo, v[i]);
        hi = std::max(hi, v[i]);
    }
    return true;
}
