# IntBuffer with Move

Your chapter 5 `IntBuffer` copies correctly. Now make it *move*: when a buffer is about to die anyway, steal its allocation instead of duplicating it. This is the mechanism behind every `std::move` you wrote in the last two problems.

## The lesson

A **move constructor** takes an rvalue reference `T&&`, takes over the source's resources, and leaves the source empty but destructible:

```cpp
IntBuffer(IntBuffer&& o) noexcept
    : data_(o.data_), n_(o.n_) {   // steal the pointer and the size: no allocation, no copying
    o.data_ = nullptr;             // the source must not delete[] what we now own
    o.n_ = 0;
}
```

Three things to see:

- `IntBuffer&&` binds only to rvalues: temporaries, and lvalues the caller wrapped in `std::move`. That is how the compiler picks copy or move with no extra syntax at the call site for temporaries.
- **Null the source.** `o` is still a live object whose destructor will run. If it still holds the pointer, two destructors `delete[]` the same block. ASan reports a double-free the moment the scope ends.
- **`noexcept` is not decoration.** When `std::vector` grows, it moves its elements into the new block, but it promises that if something throws midway the old contents are intact. It can keep that promise only if moving cannot throw, so it checks `std::is_nothrow_move_constructible` and falls back to *copying* otherwise. Leave `noexcept` off and every `push_back` that reallocates deep-copies all your buffers, silently.

**Move assignment** does the same, except `this` already owns something:

```cpp
IntBuffer& operator=(IntBuffer&& o) noexcept {
    if (this != &o) {              // a = std::move(a) through an alias must not free a's own buffer
        delete[] data_;            // release what we had
        data_ = o.data_;  n_ = o.n_;
        o.data_ = nullptr; o.n_ = 0;
    }
    return *this;
}
```

Or shorter, **move-and-swap**: `IntBuffer tmp(std::move(o)); std::swap(data_, tmp.data_); std::swap(n_, tmp.n_); return *this;`. The move constructor empties `o`, the swap installs its block in `this`, and `tmp`'s destructor frees your old buffer. Self-move safe by construction. (Swapping directly with `o`, without the temporary, also never double-frees, but it leaves `o` holding your old buffer rather than empty.)

**Rule of five.** Once you declare any of destructor, copy constructor, or copy assignment, the compiler stops generating the move operations for you. A class that manages a raw resource therefore declares all five or none (the next problem shows "none"). Declare only the three and moving silently degrades to copying: correct, slow, and invisible until you look at pointer identity.

```text
IntBuffer b(std::move(a));

before:  a.data_ ---> [7 7 7 7]               b: (not yet constructed)
after:   a.data_ = nullptr, a.n_ = 0          b.data_ ---> [7 7 7 7]   same block, nothing copied
```

A moved-from object must stay *valid*: destructible, and assignable (`a = IntBuffer(3)` after a move is normal use). Everything else about its state is your choice; empty is the convention.

## Coming from Python

- Nothing in Python corresponds to this. Names are references, so "transfer" is rebinding and the old name still works. In C++ the object's contents are relocated and the old object is hollowed out.
- There is no equivalent of `std::move`, and it is only a cast. If the class has no move constructor, `IntBuffer b(std::move(a))` quietly calls the copy constructor. The starter does exactly that; watch the identity tests.
- `noexcept` is a promise the compiler acts on, not a hint. There is no `try/except` fallback that makes an unmarked move "probably fine".

## Your task

The starter is the complete, correct rule-of-three `IntBuffer` from chapter 5 plus a `data()` accessor. Run it first: it passes the deep-copy cases and fails every identity check, because without move operations `std::move` falls back to copying.

Add the two move operations:

```cpp
class IntBuffer {
public:
    explicit IntBuffer(std::size_t n, int fill = 0);
    IntBuffer(const IntBuffer& o);
    IntBuffer& operator=(const IntBuffer& o);
    IntBuffer(IntBuffer&& o) noexcept;              // new
    IntBuffer& operator=(IntBuffer&& o) noexcept;   // new
    ~IntBuffer();
    std::size_t size() const;
    int& operator[](std::size_t i);
    const int& operator[](std::size_t i) const;
    const int* data() const;                        // the heap block, or nullptr after being moved from
private:
    int* data_;
    std::size_t n_;
};
```

| sequence | result |
|---|---|
| `const int* before = a.data(); IntBuffer b(std::move(a));` | `b.data() == before`, `a.size() == 0`, `a.data() == nullptr` |
| `c = std::move(b)` | `c.data()` is `b`'s old block, `b` is empty, `c`'s old block freed exactly once |
| `IntBuffer& r = a; a = std::move(r);` | `a` still valid and assignable |
| `v.reserve(1); v.push_back(IntBuffer(2)); keep = v[0].data(); v.push_back(IntBuffer(3));` | `v[0].data() == keep` (only with `noexcept`) |
| `IntBuffer b = a; b[0] = 99;` | `a[0]` unchanged: copying is still deep |
