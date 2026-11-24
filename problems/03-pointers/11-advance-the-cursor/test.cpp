#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    const char* s = "12ab";
    const char* cur = s;
    CHECK_EQ("read_number(&cur) with cur at the start of \"12ab\" returns 12", read_number(&cur), 12);
    CHECK_TRUE("cur == s + 2 afterward (on the 'a')", cur == s + 2);

    const char* whole = "907";
    cur = whole;
    CHECK_EQ("read_number(&cur) on \"907\" returns 907", read_number(&cur), 907);
    CHECK_TRUE("cur == whole + 3 afterward (on the terminator)", cur == whole + 3);

    const char* zeros = "007x";
    cur = zeros;
    CHECK_EQ("read_number(&cur) on \"007x\" returns 7", read_number(&cur), 7);
    CHECK_TRUE("cur == zeros + 3 afterward", cur == zeros + 3);

    cur = s + 1;
    CHECK_EQ("read_number(&cur) with cur = s + 1 in \"12ab\" returns 2", read_number(&cur), 2);
    CHECK_TRUE("cur == s + 2 afterward", cur == s + 2);

    const char* none = "abc";
    cur = none;
    CHECK_EQ("read_number(&cur) on \"abc\" returns 0", read_number(&cur), 0);
    CHECK_TRUE("cur unchanged when there are no digits", cur == none);

    const char* empty = "";
    cur = empty;
    CHECK_EQ("read_number(&cur) on \"\" returns 0", read_number(&cur), 0);
    CHECK_TRUE("cur unchanged on the empty string", cur == empty);

    // The way a caller uses it: read, step over the comma, read again.
    const char* csv = "3,14,159";
    cur = csv;
    CHECK_EQ("\"3,14,159\": first read_number(&cur) returns 3", read_number(&cur), 3);
    CHECK_TRUE("cur == csv + 1 (on the first comma)", cur == csv + 1);
    ++cur;
    CHECK_EQ("after ++cur, the second call returns 14", read_number(&cur), 14);
    CHECK_TRUE("cur == csv + 4 (on the second comma)", cur == csv + 4);
    ++cur;
    CHECK_EQ("after ++cur, the third call returns 159", read_number(&cur), 159);
    CHECK_TRUE("cur == csv + 8 (on the terminator)", cur == csv + 8);
    CHECK_EQ("one more call at the terminator returns 0", read_number(&cur), 0);
    CHECK_TRUE("cur still == csv + 8", cur == csv + 8);
}
