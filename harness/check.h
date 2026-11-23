#pragma once
// Minimal test harness. Each CHECK prints one machine-readable line:
//   @@CASE\t<name>\t<PASS|FAIL>\t<detail>
// and main() ends with:
//   @@SUMMARY\t<passed>\t<total>
// Everything else on stdout is the learner's own output.
#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace chk {
inline int passed = 0, total = 0;

template <class T> std::string show(const T& v);
inline std::string show(const std::string& s) { return "\"" + s + "\""; }
inline std::string show(const char* s) { return s ? "\"" + std::string(s) + "\"" : "nullptr"; }
inline std::string show(char c) { return std::string("'") + c + "'"; }
inline std::string show(bool b) { return b ? "true" : "false"; }
inline std::string show(std::nullptr_t) { return "nullptr"; }
template <class A, class B> std::string show(const std::pair<A, B>& p) { return "(" + show(p.first) + ", " + show(p.second) + ")"; }
template <class T> std::string show(const std::vector<T>& v) {
    std::string out = "[";
    for (std::size_t i = 0; i < v.size(); ++i) out += (i ? ", " : "") + show(v[i]);
    return out + "]";
}
template <class K, class V> std::string show(const std::map<K, V>& m) {
    std::string out = "{";
    bool first = true;
    for (const auto& kv : m) { out += (first ? "" : ", ") + show(kv.first) + ": " + show(kv.second); first = false; }
    return out + "}";
}
template <class T> std::string show(const T& v) {
    std::ostringstream o;
    o << std::boolalpha << v;
    return o.str();
}

inline void report(const std::string& name, bool ok, std::string detail) {
    ++total;
    if (ok) ++passed;
    for (char& c : detail) if (c == '\n' || c == '\t') c = ' ';
    std::cout << "@@CASE\t" << name << "\t" << (ok ? "PASS" : "FAIL") << "\t" << detail << "\n" << std::flush;
}
}  // namespace chk

#define CHECK_EQ(name, got, expected)                                                        \
    do {                                                                                     \
        auto&& g_ = (got);                                                                   \
        auto&& e_ = (expected);                                                              \
        bool ok_ = (g_ == e_);                                                               \
        chk::report(name, ok_, ok_ ? "" : "expected " + chk::show(e_) + ", got " + chk::show(g_)); \
    } while (0)

#define CHECK_TRUE(name, cond)                                                               \
    do {                                                                                     \
        bool ok_ = static_cast<bool>(cond);                                                  \
        chk::report(name, ok_, ok_ ? "" : "expected true: " #cond);                          \
    } while (0)

#define CHECK_NEAR(name, got, expected, eps)                                                 \
    do {                                                                                     \
        double g_ = static_cast<double>(got);                                                \
        double e_ = static_cast<double>(expected);                                           \
        bool ok_ = std::fabs(g_ - e_) <= (eps);                                              \
        chk::report(name, ok_, ok_ ? "" : "expected " + chk::show(e_) + " (±" + chk::show(eps) + "), got " + chk::show(g_)); \
    } while (0)

void run_tests();

int main() {
    run_tests();
    std::cout << "@@SUMMARY\t" << chk::passed << "\t" << chk::total << "\n" << std::flush;
    return chk::passed == chk::total ? 0 : 1;
}
