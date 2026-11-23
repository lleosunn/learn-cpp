#include <algorithm>
#include <iterator>
#include <set>
#include <unordered_set>
#include <vector>

void remove_all(std::vector<int>& v, int x) {
    // remove() shifts the keepers to the front and returns the new logical end;
    // erase() then drops the junk tail. Together: the erase-remove idiom.
    v.erase(std::remove(v.begin(), v.end(), x), v.end());
}

std::vector<int> dedupe_keep_order(const std::vector<int>& v) {
    std::vector<int> out;
    std::unordered_set<int> seen;
    for (int y : v)
        if (seen.insert(y).second) out.push_back(y);   // .second: "was it new?"
    return out;
}

std::vector<int> sorted_intersection(const std::vector<int>& a, const std::vector<int>& b) {
    std::set<int> sa(a.begin(), a.end()), sb(b.begin(), b.end());  // sorted and unique
    std::vector<int> out;
    std::set_intersection(sa.begin(), sa.end(), sb.begin(), sb.end(),
                          std::back_inserter(out));                 // grows out via push_back
    return out;
}
