#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

struct Student {
    std::string name;
    int score;
};

// In place: score descending, then name ascending.
void sort_students(std::vector<Student>& v) {
    // TODO: std::sort with a lambda comparator (strict: no >= or <=)
}

// Names of students with score > min_score, in input order.
std::vector<std::string> names_above(const std::vector<Student>& v, int min_score) {
    // TODO: loop, test, push_back
    return {};
}

// Arithmetic mean of the scores; 0.0 for an empty roster.
double mean_score(const std::vector<Student>& v) {
    // TODO: std::accumulate with a double seed
    return 0.0;
}

// Pointer to the first student with that name inside v, or nullptr.
const Student* find_student(const std::vector<Student>& v, const std::string& name) {
    // TODO: std::find_if, then iterator -> pointer
    return nullptr;
}
