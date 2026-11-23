#include <algorithm>
#include <cstddef>

class IntStack {
public:
    IntStack() : data_(nullptr), size_(0), cap_(0) {}
    ~IntStack() { delete[] data_; }  // nothing can observe a missing delete[] here; write it anyway

    IntStack(const IntStack&) = delete;             // the default copy would share data_ -> double delete
    IntStack& operator=(const IntStack&) = delete;

    void push(int v) {
        if (size_ == cap_) grow();  // data_[size_] is only writable when size_ < cap_
        data_[size_++] = v;
    }

    bool pop(int& out) {
        if (size_ == 0) return false;
        out = data_[--size_];  // no free, no shrink: capacity stays for the next push
        return true;
    }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return cap_; }

private:
    void grow() {
        std::size_t new_cap = cap_ ? cap_ * 2 : 1;  // geometric growth keeps push amortized O(1)
        int* p = new int[new_cap];                  // 1. allocate
        std::copy(data_, data_ + size_, p);         // 2. copy the live elements (size_, not cap_)
        delete[] data_;                             // 3. release the old block
        data_ = p;
        cap_ = new_cap;
    }

    int* data_;
    std::size_t size_;
    std::size_t cap_;
};
