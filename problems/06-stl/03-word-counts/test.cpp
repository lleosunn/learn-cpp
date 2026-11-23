#include "check.h"
#include "tracked.h"
#include "solution.cpp"

using Counts = std::map<std::string, int>;
using Ranked = std::vector<std::pair<std::string, int>>;

void run_tests() {
    CHECK_EQ("word_counts({b,a,b})", word_counts({"b", "a", "b"}), (Counts{{"a", 1}, {"b", 2}}));
    CHECK_EQ("word_counts({})", word_counts({}), (Counts{}));
    CHECK_EQ("word_counts({x})", word_counts({"x"}), (Counts{{"x", 1}}));
    CHECK_EQ("word_counts is sorted by key", word_counts({"pear", "apple", "fig", "apple"}),
             (Counts{{"apple", 2}, {"fig", 1}, {"pear", 1}}));

    Counts c{{"a", 2}, {"b", 2}, {"c", 1}};
    CHECK_EQ("top_k(k=2): tie broken alphabetically", top_k(c, 2), (Ranked{{"a", 2}, {"b", 2}}));
    CHECK_EQ("top_k(k=10) on 3 words returns all 3", top_k(c, 10), (Ranked{{"a", 2}, {"b", 2}, {"c", 1}}));
    CHECK_EQ("top_k(k=0)", top_k(c, 0), (Ranked{}));
    CHECK_EQ("top_k on empty counts", top_k(Counts{}, 3), (Ranked{}));

    Counts skewed{{"apple", 1}, {"zebra", 5}};
    CHECK_EQ("top_k sorts by count, not by word", top_k(skewed, 1), (Ranked{{"zebra", 5}}));

    Ranked got = top_k(word_counts({"the", "cat", "the", "dog", "the", "cat"}), 2);
    CHECK_EQ("end to end: top 2 of a sentence", got, (Ranked{{"the", 3}, {"cat", 2}}));
}
