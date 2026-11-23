#include <cstddef>

// Number of chars in s before the '\0' terminator.
std::size_t c_strlen(const char* s) {
    // TODO: walk a pointer forward until *p == '\0', then return the distance from s
    const char* p = s;
    while (*p != '\0') ++p;
    return p - s;
}

// Pointer to the first c in s, or nullptr if absent. c == '\0' finds the terminator.
const char* find_char(const char* s, char c) {
    while (true) {
        if (*s == c) return s;
        if (*s == '\0') return nullptr;
        ++s;
    }
}
