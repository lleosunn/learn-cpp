#include <cstddef>

// Copy src into dst, which has room for cap bytes in total (terminator included).
// Copy at most cap - 1 chars, always terminate when cap > 0, return the number of chars copied.
std::size_t copy_string(char* dst, std::size_t cap, const char* src) {
    // TODO: handle cap == 0, copy while there is room, then write the '\0'
    if (cap == 0) {
        return 0;
    }
    int copied = 0;
    for (std::size_t i = 0; i < cap - 1; ++i) {
        if (*src != '\0') {
            *dst = *src;
            ++dst;
            ++src;
            ++copied;
        }
        else break;
    }
    *dst = '\0';
    return copied;
}
