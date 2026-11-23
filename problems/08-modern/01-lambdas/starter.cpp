#include <functional>
#include <vector>

// Returns a function f such that f(x) == x + n.
std::function<int(int)> make_adder(int n) {
    // TODO: capture n by value
    return [](int) { return 0; };
}

// Returns a function that yields 1, 2, 3, ... on successive calls.
std::function<int()> make_counter() {
    // TODO: init-capture a counter and make the lambda mutable
    return []() { return 0; };
}

// Returns a function that pushes `value` into `sink` every time it is called.
std::function<void()> make_appender(std::vector<int>& sink, int value) {
    // TODO: capture sink by reference and value by copy
    return []() {};
}
