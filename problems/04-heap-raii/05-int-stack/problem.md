# Growable Stack on a Raw Buffer

Everything from this chapter in one class: a block from `new[]`, growth from problem 3, and a destructor from problem 4. This is `list.append` with the lid off.

## The lesson

A growable container keeps two counts, not one:

```text
data_ --> [ 3 | 7 | 1 | ? | ? | ? | ? | ? ]     heap block from new int[cap_]
            <- size_=3 ->
            <------------ cap_ = 8 ----------->
```

- `size_` is how many slots hold real values. `push` writes `data_[size_]` and increments; `pop` decrements and reads.
- `cap_` is how many slots the block has. Writing `data_[size_]` is only legal when `size_ < cap_`. When `size_ == cap_` you must grow *before* writing; `data_[cap_]` is one past the end, and AddressSanitizer reports a heap-buffer-overflow.

**Growing** is problem 3 with `int`: allocate `new_cap`, copy the first `size_` values (not `cap_`, and certainly not `cap_ + 1`), `delete[]` the old block, point `data_` at the new one, update `cap_`.

**Why double?** If you grew by one slot per push, the n-th push would copy n values and n pushes would copy about n²/2 values in total. Doubling (`0 → 1 → 2 → 4 → 8 → ...`) means a push that copies k values is followed by k cheap pushes, so the total copy work for n pushes stays under 2n. That is "amortized O(1)". It is also why `pop` must not shrink the block: if a pop at a boundary freed half, a push/pop/push/pop sequence at that boundary would copy on every call and the argument collapses.

**The destructor** is `delete[] data_`. Be honest about one thing: nothing here can observe whether you wrote it. `int` has no destructor to count, and macOS has no leak detector. Write it anyway; the point of this chapter is that the habit precedes the tool that checks it. Chapter 6 introduces `std::vector`, which is this class done properly. After that you will never write `IntStack` again, but you will know exactly what `push_back` costs and why `capacity()` exists.

Two C++ details:

- Members are **not** zero-initialized. The constructor's initializer list `IntStack() : data_(nullptr), size_(0), cap_(0) {}` is what makes them zero; without it they hold whatever was in that memory.
- Copying is forbidden with `= delete`, for the same reason as problem 4: the default copy would share `data_` and double-delete. Chapter 5 writes the proper copy.

## Coming from Python

- `list.append` is amortized O(1) for exactly this reason. CPython over-allocates by roughly an eighth rather than doubling, but the shape of the argument is the same.
- `pop` returns `bool` and writes through an `int&` instead of raising `IndexError`. C++ has exceptions, but for "was it empty" a boolean is the lighter tool, and it is the Chapter 2 reference-parameter pattern in action.
- `std::copy(first, last, dest)` from `<algorithm>` is a `for` loop in disguise; a hand-written loop is equally fine.

## Your task

Implement `IntStack`. Capacity grows `0 → 1 → 2 → 4 → 8 → ...`, only when a push finds the block full, and never shrinks. `pop` returns `false` and leaves `out` untouched when the stack is empty.

```cpp
class IntStack {
public:
    IntStack();
    ~IntStack();
    IntStack(const IntStack&) = delete;
    IntStack& operator=(const IntStack&) = delete;
    void push(int v);
    bool pop(int& out);
    std::size_t size() const;
    std::size_t capacity() const;
private:
    int* data_;
    std::size_t size_;
    std::size_t cap_;
};
```

| sequence | result |
|---|---|
| `push(1)` through `push(5)` | `size() == 5`, `capacity() == 8` |
| then `pop(out)` repeatedly | `5, 4, 3, 2, 1`, then `false`; `capacity()` still `8` |
| fresh stack, `pop(out)` | `false`, `out` unchanged |

Writing `data_[cap_]`, or copying `cap_ + 1` elements in `grow`, is a heap-buffer-overflow; the tests abort with a sanitizer report pointing at the line. If the very first push writes through the still-null `data_` (you never grew), UBSan reports a null-pointer store instead; same cause, read it the same way.
