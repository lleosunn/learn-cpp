int read_number(const char** cursor) {
    const char* p = *cursor;  // copy in: the caller's current position
    int value = 0;
    while (*p >= '0' && *p <= '9') {      // '\0' is not a digit, so this stops at the terminator too
        value = value * 10 + (*p - '0');  // shift one decimal place, add the new digit
        ++p;
    }
    *cursor = p;  // copy out: the caller's pointer now sits on the first non-digit
    return value;
}
