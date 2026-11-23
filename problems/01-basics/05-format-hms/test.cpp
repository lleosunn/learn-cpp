#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    CHECK_EQ("format_hms(0)", format_hms(0), std::string("0:00:00"));
    CHECK_EQ("format_hms(59)", format_hms(59), std::string("0:00:59"));
    CHECK_EQ("format_hms(60)", format_hms(60), std::string("0:01:00"));
    CHECK_EQ("format_hms(3600)", format_hms(3600), std::string("1:00:00"));
    CHECK_EQ("format_hms(3723)", format_hms(3723), std::string("1:02:03"));
    CHECK_EQ("format_hms(86399)", format_hms(86399), std::string("23:59:59"));
    CHECK_EQ("format_hms(90000)", format_hms(90000), std::string("25:00:00"));
    CHECK_EQ("format_hms(360000)", format_hms(360000), std::string("100:00:00"));
}
