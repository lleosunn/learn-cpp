#include "check.h"
#include "tracked.h"
#include "solution.cpp"

static std::vector<std::string> names(const std::vector<Student>& v) {
    std::vector<std::string> out;
    for (const Student& s : v) out.push_back(s.name);
    return out;
}

void run_tests() {
    std::vector<Student> v{{"bob", 80}, {"amy", 90}, {"cat", 80}};

    CHECK_EQ("names_above(v, 85)", names_above(v, 85), (std::vector<std::string>{"amy"}));
    CHECK_EQ("names_above(v, 79) keeps input order", names_above(v, 79), (std::vector<std::string>{"bob", "amy", "cat"}));
    CHECK_EQ("names_above(v, 80) is strict", names_above(v, 80), (std::vector<std::string>{"amy"}));
    CHECK_EQ("names_above(v, 90) is empty", names_above(v, 90), (std::vector<std::string>{}));

    CHECK_NEAR("mean_score({80,90,80})", mean_score(v), 250.0 / 3.0, 1e-9);
    CHECK_NEAR("mean_score({90,80})", mean_score({{"a", 90}, {"b", 80}}), 85.0, 1e-9);
    CHECK_NEAR("mean_score({1,2}) is 1.5, not 1", mean_score({{"a", 1}, {"b", 2}}), 1.5, 1e-9);
    CHECK_NEAR("mean_score({}) is 0.0", mean_score({}), 0.0, 1e-9);

    CHECK_TRUE("find_student(v, \"cat\") points into v", find_student(v, "cat") == &v[2]);
    CHECK_TRUE("find_student(v, \"bob\") points into v", find_student(v, "bob") == &v[0]);
    CHECK_TRUE("find_student(v, \"dan\") is nullptr", find_student(v, "dan") == nullptr);

    sort_students(v);
    CHECK_EQ("sort_students: score desc, name asc", names(v), (std::vector<std::string>{"amy", "bob", "cat"}));

    std::vector<Student> mixed{{"zed", 70}, {"amy", 95}, {"bob", 95}, {"cal", 70}, {"dee", 100}};
    sort_students(mixed);
    CHECK_EQ("sort_students: mixed roster", names(mixed), (std::vector<std::string>{"dee", "amy", "bob", "cal", "zed"}));

    // 1000 students with the same score: a comparator using >= is UB here and usually crashes.
    std::vector<Student> tied;
    for (int i = 999; i >= 0; --i) tied.push_back({std::to_string(1000 + i), 50});
    sort_students(tied);
    bool ascending = true;
    for (std::size_t i = 1; i < tied.size(); ++i)
        if (tied[i - 1].name > tied[i].name) ascending = false;
    CHECK_TRUE("sort_students: 1000 tied scores sorted by name", ascending && tied.size() == 1000);
}
