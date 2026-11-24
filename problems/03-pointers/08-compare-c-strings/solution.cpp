bool c_streq(const char* a, const char* b) {
    for (;; ++a, ++b) {
        if (*a != *b) return false;   // also covers "one string ended first"
        if (*a == '\0') return true;  // equal and at the terminator: both ended together
    }
}

bool starts_with(const char* s, const char* prefix) {
    for (; *prefix != '\0'; ++s, ++prefix) {
        if (*s != *prefix) return false;  // if s ends first, its '\0' mismatches here
    }
    return true;  // ran out of prefix without a mismatch
}
