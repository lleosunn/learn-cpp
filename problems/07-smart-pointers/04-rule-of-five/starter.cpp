#include <algorithm>
#include <cstddef>
#include <utility>

// Chapter 5's rule-of-three IntBuffer, complete and correct, plus data().
// TODO: add the move constructor and move assignment (both noexcept).
class IntBuffer {
public:
    explicit IntBuffer(std::size_t n, int fill = 0) : data_(new int[n]), n_(n) {
        std::fill(data_, data_ + n_, fill);
    }

    IntBuffer(const IntBuffer& o) : data_(new int[o.n_]), n_(o.n_) {
        std::copy(o.data_, o.data_ + n_, data_);
    }

    IntBuffer& operator=(const IntBuffer& o) {
        if (this != &o) {
            int* fresh = new int[o.n_];
            std::copy(o.data_, o.data_ + o.n_, fresh);
            delete[] data_;
            data_ = fresh;
            n_ = o.n_;
        }
        return *this;
    }

    // TODO: IntBuffer(IntBuffer&& o) noexcept
    // TODO: IntBuffer& operator=(IntBuffer&& o) noexcept

    ~IntBuffer() { delete[] data_; }

    std::size_t size() const { return n_; }
    int& operator[](std::size_t i) { return data_[i]; }
    const int& operator[](std::size_t i) const { return data_[i]; }
    const int* data() const { return data_; }

private:
    int* data_;
    std::size_t n_;
};
