#include <string>

// Two-digit, zero-padded text for 0 <= v < 100. Defined above its caller:
// C++ must see a declaration before the first call.
std::string pad2(int v) {
    std::string s = std::to_string(v);
    if (v < 10) s = "0" + s;   // OK: s is a std::string. "0" + v would be pointer arithmetic.
    return s;
}

std::string format_hms(int total_seconds) {
    const int h = total_seconds / 3600;
    const int m = total_seconds % 3600 / 60;   // what is left after whole hours, in minutes
    const int s = total_seconds % 60;
    return std::to_string(h) + ":" + pad2(m) + ":" + pad2(s);
}
