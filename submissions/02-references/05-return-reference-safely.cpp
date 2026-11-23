#include <string>
#include <vector>

// Return a reference to the longest string in words (first wins ties).
// words is non-empty. Do not copy the string.
// This version compiles with a warning. Run it and read what the sanitizer says.
const std::string& longest_word(const std::vector<std::string>& words) {
    std::size_t best_index = 0;
    for (std::size_t i = 0; i < words.size(); ++i) {
        if (words[i].size() > words[best_index].size()){
            best_index = i;
        }
    }
    return words[best_index];
}
