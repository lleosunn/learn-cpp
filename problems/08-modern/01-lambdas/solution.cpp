#include <functional>
#include <vector>

std::function<int(int)> make_adder(int n) {
    return [n](int x) { return x + n; };  // n is copied into the closure
}

std::function<int()> make_counter() {
    // init-capture creates a variable owned by the closure; mutable lets the body change it
    return [count = 0]() mutable { return ++count; };
}

std::function<void()> make_appender(std::vector<int>& sink, int value) {
    // sink is the caller's vector and outlives the closure, so a reference is safe;
    // value is a plain int, copy it
    return [&sink, value]() { sink.push_back(value); };
}
