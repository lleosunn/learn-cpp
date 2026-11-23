#include <cstddef>
#include <vector>

std::vector<int> running_sum(const std::vector<int>& v) {
    std::vector<int> out;
    out.reserve(v.size());             // one allocation up front; size() is still 0
    int acc = 0;
    for (int x : v) out.push_back(acc += x);
    return out;                        // returned by value, no copy in practice
}

bool get_at(const std::vector<int>& v, std::size_t i, int& out) {
    if (i >= v.size()) return false;   // the check operator[] does not do for you
    out = v[i];
    return true;
}
