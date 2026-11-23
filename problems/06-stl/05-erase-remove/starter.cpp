#include <algorithm>
#include <iterator>
#include <set>
#include <unordered_set>
#include <vector>

// Remove every occurrence of x from v, in place, keeping the order of the rest.
void remove_all(std::vector<int>& v, int x) {
    // TODO: this does not compile. A range-for hands you the value, not an
    // iterator, so there is nothing to pass to erase(). Read the lesson, then
    // replace this loop with the erase-remove idiom.
    for (int y : v)
        if (y == x) v.erase(/* ??? */);
}

// Distinct values of v, in order of first appearance.
std::vector<int> dedupe_keep_order(const std::vector<int>& v) {
    // TODO: unordered_set<int> seen; insert(y).second tells you if y is new
    return {};
}

// Values present in both a and b: ascending, no duplicates. Inputs may be unsorted.
std::vector<int> sorted_intersection(const std::vector<int>& a, const std::vector<int>& b) {
    // TODO: std::set from each, then std::set_intersection into a back_inserter
    return {};
}
