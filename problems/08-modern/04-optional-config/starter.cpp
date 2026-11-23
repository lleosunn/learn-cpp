#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

// Strict integer parse: optional leading '-', one or more digits, nothing else,
// and the value must fit in an int. Anything else -> std::nullopt.
std::optional<int> parse_int(const std::string& s) {
    // TODO
    return std::nullopt;
}

// "key=<int>" with a non-empty key and a strict integer after the first '='.
// No trimming. Anything else -> std::nullopt.
std::optional<std::pair<std::string, int>> parse_kv(const std::string& line) {
    // TODO
    return std::nullopt;
}

// Parse every line with parse_kv, skip invalid ones; a later line wins for a repeated key.
std::map<std::string, int> parse_config(const std::vector<std::string>& lines) {
    // TODO
    return {};
}
