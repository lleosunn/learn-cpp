#include <algorithm>
#include <cstddef>

// A stack of ints on a heap block that doubles when it fills: 0 -> 1 -> 2 -> 4 -> ...
class IntStack {
public:
    IntStack() : data_(nullptr), size_(0), cap_(0) {}
    ~IntStack() {
        // TODO: release the block
    }

    // TODO: forbid copying

    void push(int v) {
        // TODO: grow if full, then store v
    }

    // Remove the top into out. Return false (and leave out alone) if empty.
    bool pop(int& out) {
        // TODO
        return false;
    }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return cap_; }

private:
    void grow() {
        // TODO: allocate, copy, free old, repoint (problem 3 with ints)
    }

    int* data_;
    std::size_t size_;
    std::size_t cap_;
};
