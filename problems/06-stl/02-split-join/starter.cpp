#include <cctype>
#include <string>
#include <vector>

// Python's s.split(delim): every delimiter is a boundary, empty pieces are kept.
std::vector<std::string> split(const std::string& s, char delim) {
    // TODO: cursor loop with s.find(delim, start) and s.substr(start, len)
    return {};
}

// Python's sep.join(parts).
std::string join(const std::vector<std::string>& parts, const std::string& sep) {
    // TODO: += each part, with sep before every part except the first
    return "";
}

// Python's s.upper(), ASCII only.
std::string to_upper(std::string s) {
    // TODO: mutate the copy in place (cast through unsigned char for std::toupper)
    return s;
}
