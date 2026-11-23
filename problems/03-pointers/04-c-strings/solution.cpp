#include <cstddef>

std::size_t c_strlen(const char* s) {
    const char* p = s;
    while (*p != '\0') ++p;                   // walk until the terminator
    return static_cast<std::size_t>(p - s);   // p - s is a ptrdiff_t; never negative here
}

const char* find_char(const char* s, char c) {
    for (;; ++s) {
        if (*s == c) return s;        // test c first, so c == '\0' finds the terminator
        if (*s == '\0') return nullptr;
    }
}
