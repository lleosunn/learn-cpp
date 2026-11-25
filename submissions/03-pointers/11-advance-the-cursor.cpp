// Read the run of decimal digits starting at *cursor and return its value.
// Leave *cursor on the first non-digit. No digits: return 0 and leave *cursor unchanged.
int read_number(const char** cursor) {
    // TODO: copy *cursor into a local pointer, walk the digits, write the position back
    int value = 0;
    while ('0' <= **cursor && **cursor <= '9') {
        value = 10 * value + (**cursor - '0');
        ++*cursor;
    }
    return value;
}
