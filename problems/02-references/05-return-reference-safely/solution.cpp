#include <string>
#include <vector>

const std::string& longest_word(const std::vector<std::string>& words) {
    std::size_t best = 0;  // an index into words, not a copy of a string
    for (std::size_t i = 1; i < words.size(); ++i)
        if (words[i].size() > words[best].size()) best = i;  // strictly longer: first wins ties
    return words[best];    // refers into the caller's vector, which outlives this call
}
