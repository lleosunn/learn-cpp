#include "tracked.h"

class ScopedTracked {
public:
    explicit ScopedTracked(int v) : p_(new Tracked(v)) {}  // acquire in the constructor
    ~ScopedTracked() { delete p_; }                        // release in the destructor, on every exit path

    // The default copy would duplicate p_ and delete it twice.
    ScopedTracked(const ScopedTracked&) = delete;
    ScopedTracked& operator=(const ScopedTracked&) = delete;

    Tracked& get() { return *p_; }
    const Tracked& get() const { return *p_; }

    Tracked* release() {
        Tracked* t = p_;
        p_ = nullptr;  // destructor now deletes nothing; the caller owns t
        return t;
    }

private:
    Tracked* p_;
};
