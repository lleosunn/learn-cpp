#pragma once
#include <ostream>

// Tracked: an object that counts its own lifetime events, so tests can prove
// "no leak" (alive == 0 after a scope), "no unnecessary copies" (copies == 0),
// or "was moved, not copied" — without a leak sanitizer (unavailable on macOS).
struct Tracked {
    int value;
    static inline int alive = 0, constructed = 0, destroyed = 0, copies = 0, moves = 0;
    static void reset() { alive = constructed = destroyed = copies = moves = 0; }

    Tracked(int v = 0) : value(v) { ++alive; ++constructed; }
    Tracked(const Tracked& o) : value(o.value) { ++alive; ++constructed; ++copies; }
    Tracked(Tracked&& o) noexcept : value(o.value) { ++alive; ++constructed; ++moves; }
    Tracked& operator=(const Tracked& o) { value = o.value; ++copies; return *this; }
    Tracked& operator=(Tracked&& o) noexcept { value = o.value; ++moves; return *this; }
    ~Tracked() { --alive; ++destroyed; }
};

inline bool operator==(const Tracked& a, const Tracked& b) { return a.value == b.value; }
inline std::ostream& operator<<(std::ostream& os, const Tracked& t) { return os << "Tracked(" << t.value << ")"; }
