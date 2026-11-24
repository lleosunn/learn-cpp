#include <cstddef>

// Copy src into dst, which has room for cap bytes in total (terminator included).
// Copy at most cap - 1 chars, always terminate when cap > 0, return the number of chars copied.
std::size_t copy_string(char* dst, std::size_t cap, const char* src) {
    // TODO: handle cap == 0, copy while there is room, then write the '\0'
    return 0;
}
