#include <algorithm>
#include <cstddef>
#include <utility>

class IntBuffer {
public:
    explicit IntBuffer(std::size_t n, int fill = 0) : data_(new int[n]), n_(n) {
        std::fill(data_, data_ + n_, fill);
    }

    // Copy constructor: our own array, same contents.
    IntBuffer(const IntBuffer& o) : data_(new int[o.n_]), n_(o.n_) {
        std::copy(o.data_, o.data_ + n_, data_);
    }

    // Copy assignment, copy-and-swap: build the copy first, swap it in, and let
    // tmp's destructor free the old array. Safe for a = a because o is never
    // touched after tmp is built.
    IntBuffer& operator=(const IntBuffer& o) {
        IntBuffer tmp(o);
        std::swap(data_, tmp.data_);
        std::swap(n_, tmp.n_);
        return *this;   // returning *this is what makes a = b = c work
    }

    ~IntBuffer() { delete[] data_; }

    std::size_t size() const { return n_; }
    int& operator[](std::size_t i) { return data_[i]; }
    const int& operator[](std::size_t i) const { return data_[i]; }

private:
    int* data_;
    std::size_t n_;
};
