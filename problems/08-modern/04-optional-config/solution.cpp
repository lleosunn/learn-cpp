#include <limits>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

std::optional<int> parse_int(const std::string& s) {
    std::size_t i = 0;
    bool neg = false;
    if (i < s.size() && s[i] == '-') { neg = true; ++i; }
    if (i == s.size()) return std::nullopt;  // "" or "-": no digits
    // magnitude limit: 2147483647 for positive, 2147483648 for negative
    const long long limit = neg ? -static_cast<long long>(std::numeric_limits<int>::min())
                                : std::numeric_limits<int>::max();
    long long mag = 0;
    for (; i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') return std::nullopt;
        mag = mag * 10 + (s[i] - '0');
        if (mag > limit) return std::nullopt;  // bail early so mag never overflows
    }
    return static_cast<int>(neg ? -mag : mag);
}

std::optional<std::pair<std::string, int>> parse_kv(const std::string& line) {
    auto eq = line.find('=');
    if (eq == std::string::npos || eq == 0) return std::nullopt;  // no '=' or empty key
    auto v = parse_int(line.substr(eq + 1));                       // "1=2" fails here
    if (!v) return std::nullopt;
    return std::pair{line.substr(0, eq), *v};
}

std::map<std::string, int> parse_config(const std::vector<std::string>& lines) {
    std::map<std::string, int> cfg;
    for (const auto& line : lines)
        if (auto kv = parse_kv(line)) {  // declare + test: skips invalid lines
            auto [k, v] = *kv;           // structured binding over the pair
            cfg[k] = v;                  // later lines overwrite earlier ones
        }
    return cfg;
}
