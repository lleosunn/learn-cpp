#include <cctype>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> out;
    std::size_t start = 0, pos;
    while ((pos = s.find(delim, start)) != std::string::npos) {
        out.push_back(s.substr(start, pos - start));  // the piece before this delimiter
        start = pos + 1;                              // skip the delimiter itself
    }
    out.push_back(s.substr(start));                   // the final piece, even if empty
    return out;
}

std::string join(const std::vector<std::string>& parts, const std::string& sep) {
    std::string out;
    for (std::size_t i = 0; i < parts.size(); ++i) {
        if (i) out += sep;                            // separator before all but the first
        out += parts[i];
    }
    return out;
}

std::string to_upper(std::string s) {                 // by value: s is already our own copy
    for (char& c : s)
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}
