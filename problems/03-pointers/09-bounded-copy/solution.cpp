#include <cstddef>

std::size_t copy_string(char* dst, std::size_t cap, const char* src) {
    if (cap == 0) return 0;  // no room even for the terminator; dst may be null
    std::size_t i = 0;
    while (i + 1 < cap && src[i] != '\0') {  // i + 1 < cap leaves one byte for the '\0'
        dst[i] = src[i];
        ++i;
    }
    dst[i] = '\0';  // i <= cap - 1 here, so this write is always in bounds
    return i;
}
