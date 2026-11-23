#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

struct Student {
    std::string name;
    int score;
};

void sort_students(std::vector<Student>& v) {
    std::sort(v.begin(), v.end(), [](const Student& a, const Student& b) {
        if (a.score != b.score) return a.score > b.score;  // higher score first
        return a.name < b.name;                            // tie: alphabetical
    });
}

std::vector<std::string> names_above(const std::vector<Student>& v, int min_score) {
    std::vector<std::string> out;
    for (const Student& s : v)
        if (s.score > min_score) out.push_back(s.name);    // filter + project in one pass
    return out;
}

double mean_score(const std::vector<Student>& v) {
    if (v.empty()) return 0.0;
    double total = std::accumulate(v.begin(), v.end(), 0.0,   // 0.0: the seed sets the result type
        [](double acc, const Student& s) { return acc + s.score; });
    return total / static_cast<double>(v.size());
}

const Student* find_student(const std::vector<Student>& v, const std::string& name) {
    auto it = std::find_if(v.begin(), v.end(),
        [&](const Student& s) { return s.name == name; });    // [&] captures `name`
    return it == v.end() ? nullptr : &*it;                    // iterator -> pointer into v
}
