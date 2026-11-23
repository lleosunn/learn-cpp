#include <vector>

// const& parameter: alias the caller's vector, promise not to modify it.
int sum_values(const std::vector<Tracked>& items) {
    int s = 0;
    for (const Tracked& t : items) s += t.value;  // const& loop variable: no per-element copy
    return s;
}
