#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

std::map<std::string, int> word_counts(const std::vector<std::string>& words) {
    std::map<std::string, int> counts;
    for (const std::string& w : words) ++counts[w];   // operator[] inserts 0 on first sight
    return counts;
}

std::vector<std::pair<std::string, int>> top_k(const std::map<std::string, int>& counts, std::size_t k) {
    // A map is fixed in key order; copy the pairs out so we can sort them by count.
    std::vector<std::pair<std::string, int>> v(counts.begin(), counts.end());
    std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second > b.second;  // count descending
        return a.first < b.first;                              // tie: word ascending
    });
    v.resize(std::min(k, v.size()));                           // clamp so resize never pads
    return v;
}
