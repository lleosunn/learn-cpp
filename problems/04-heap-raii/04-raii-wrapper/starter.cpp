#include "tracked.h"

// Owns one heap-allocated Tracked for exactly as long as the wrapper lives.
class ScopedTracked {
public:
    explicit ScopedTracked(int v) : p_(new Tracked(v)) {}  // acquire

    ~ScopedTracked() {
        // TODO: release what the constructor acquired
    }

    // TODO: forbid copying (two wrappers must never own one object)

    Tracked& get() { return *p_; }
    const Tracked& get() const { return *p_; }

    // Give up ownership: return the pointer; the wrapper owns nothing afterwards.
    Tracked* release() {
        // TODO
        return nullptr;
    }

private:
    Tracked* p_;
};
