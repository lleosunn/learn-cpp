#include <algorithm>
#include <string>
#include <utility>
#include <vector>

template <typename T>
T max_of(const std::vector<T>& v) {
    return *std::max_element(v.begin(), v.end());  // uses only operator<
}

template <typename T>
std::pair<T, T> min_max(const std::vector<T>& v) {
    auto [lo, hi] = std::minmax_element(v.begin(), v.end());  // pair of iterators
    return {*lo, *hi};
}

template <typename T, typename Pred>
std::vector<T> filter(const std::vector<T>& v, Pred keep) {
    std::vector<T> out;
    for (const T& x : v)
        if (keep(x)) out.push_back(x);  // Pred is whatever callable the caller passed
    return out;
}

constexpr long long ipow(long long base, int exp) {  // constexpr: callable at compile time
    long long r = 1;
    while (exp-- > 0) r *= base;
    return r;
}
