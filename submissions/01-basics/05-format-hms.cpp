#include <string>

// Format total_seconds (>= 0) as H:MM:SS: hours unpadded, minutes and seconds two digits.
std::string pad2(int val) {
    if (val < 10) {
        return "0" + std::to_string(val);
    }
    else {
        return std::to_string(val);
    }
}

std::string format_hms(int total_seconds) {
    // TODO: split with / and %, build the string with std::to_string and +.
    //       A pad2(int) helper defined ABOVE this function keeps it readable.
    int hours = total_seconds / 3600;
    total_seconds = total_seconds - 3600 * hours;
    int minutes = total_seconds / 60;
    total_seconds = total_seconds - 60 * minutes;
    return std::to_string(hours) + ":" + pad2(minutes) + ":" + pad2(total_seconds);
}
