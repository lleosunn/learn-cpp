// True if a and b have the same length and the same chars.
// This starter is deliberately wrong: it compares the two addresses. Run it first.
bool c_streq(const char* a, const char* b) {
    // TODO: walk both pointers together and compare *a with *b
    while (*a == *b) {
        if (*a == '\0') return true;
        ++a;
        ++b;
    }
    return false;
}

// True if s begins with all of prefix. An empty prefix always matches.
bool starts_with(const char* s, const char* prefix) {
    // TODO: walk both until prefix hits its terminator
    while (true) {
        if (*prefix == '\0') {
            return true;
        }
        else if (*s == *prefix) {
            ++s;
            ++prefix;
            continue;
        }
        else return false;
    }
}
