#include <algorithm>
#include <cstddef>
#include <utility>

class IntBuffer {
public:
    explicit IntBuffer(std::size_t n, int fill = 0) : data_(new int[n]), n_(n) {
        std::fill(data_, data_ + n_, fill);
    }

    // TODO: this is the shallow copy the compiler would have generated. It
    // compiles. Run the tests first and read the AddressSanitizer report.
    IntBuffer(const IntBuffer& o) : data_(o.data_), n_(o.n_) {}

    // TODO: same problem, plus the old array of *this is never freed
    IntBuffer& operator=(const IntBuffer& o) {
        data_ = o.data_;
        n_ = o.n_;
        return *this;
    }

    ~IntBuffer() { delete[] data_; }

    std::size_t size() const { return n_; }
    int& operator[](std::size_t i) { return data_[i]; }
    const int& operator[](std::size_t i) const { return data_[i]; }

private:
    int* data_;
    std::size_t n_;
};
