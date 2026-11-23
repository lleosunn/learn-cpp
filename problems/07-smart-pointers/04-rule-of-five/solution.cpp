#include <algorithm>
#include <cstddef>
#include <utility>

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

    // Move constructor: take the block, hollow out the source. noexcept lets std::vector move on growth.
    IntBuffer(IntBuffer&& o) noexcept : data_(o.data_), n_(o.n_) {
        o.data_ = nullptr;   // without this, o's destructor would delete[] our block too
        o.n_ = 0;
    }

    // Move assignment: release what we own, then take the source's block.
    IntBuffer& operator=(IntBuffer&& o) noexcept {
        if (this != &o) {    // self-move through an alias must not free our own buffer
            delete[] data_;
            data_ = o.data_;
            n_ = o.n_;
            o.data_ = nullptr;
            o.n_ = 0;
        }
        return *this;        // (alternative: move-and-swap through a temporary; a bare swap with o would leave o non-empty)
    }

    ~IntBuffer() { delete[] data_; }   // delete[] nullptr is a no-op, so a moved-from object is safe

    std::size_t size() const { return n_; }
    int& operator[](std::size_t i) { return data_[i]; }
    const int& operator[](std::size_t i) const { return data_[i]; }
    const int* data() const { return data_; }

private:
    int* data_;
    std::size_t n_;
};
