# Allocate an Array of Objects

One object was `new`/`delete`. A block of `n` objects is `new[]`/`delete[]`, and the brackets are not decoration.

## The lesson

`new T[n]` allocates `n` objects of type `T` side by side on the heap, runs `T`'s default constructor on each one, and returns a pointer to the first. You index it exactly like the arrays from Chapter 3:

```cpp
Tracked* p = new Tracked[3];   // three Tracked, each value == 0
p[1].value = 42;               // same as (*(p + 1)).value = 42
delete[] p;                    // three destructors, then the block is freed
```

Things to know:

- **`delete[]`, not `delete`.** `new[]` hides the element count in a small header just before the first element (the "array cookie") so `delete[]` knows how many destructors to run. Plain `delete` does not look for that header and hands the wrong address to the allocator. For `Tracked[]` AddressSanitizer catches this as `attempting free on address which was not malloc()-ed`. For `int[]` there is no destructor, so no cookie, and on macOS the mismatch runs silently unless you set `ASAN_OPTIONS=alloc_dealloc_mismatch=1`. Silent is not the same as correct.
- **You cannot pass per-element constructor arguments.** `new Tracked[n]` default-constructs everything; you fill in values afterward with a loop.
- **`n == 0` is legal.** You get back a valid, unique, non-null pointer to zero elements. It still needs its `delete[]`.
- **The count does not travel with the pointer.** `p` is just an address. `p[n]` is one past the end, and reading or writing it is a heap-buffer-overflow. Whoever receives a `T*` must also be told `n`. That is why every function in this chapter takes a size parameter, and why Chapter 6's `std::vector` bundles the two together.

Why this matters: the `delete`/`delete[]` mismatch and the off-by-one at `p[n]` are two of the most common memory bugs in C-style code, and both compile cleanly. Keeping the sanitizer on while you learn is how you build the habit before it costs you a weekend.

## Coming from Python

- `[Tracked(i) for i in range(n)]` allocates, constructs, and remembers `n` in one expression. C++ gives you a raw block and a contract: you remember `n`, you call `delete[]`.
- A Python list is resizable; a `new[]` block is not. Problem 3 is about what you do when it fills up.
- `len(p)` does not exist. A pointer has no length.

## Your task

`Tracked` is already defined for you (same interface as problem 1; the tests use its counters because macOS has no leak detector):

```cpp
struct Tracked {
    int value;
    static int alive, constructed, destroyed, copies, moves;
    static void reset();
    Tracked(int v = 0);
};
```

`make_tracked_array(n, first_value)` returns a heap array of `n` `Tracked` where element `i` has `value == first_value + i`. `n` may be `0`. `free_tracked_array` releases such an array.

```cpp
Tracked* make_tracked_array(std::size_t n, int first_value);
void free_tracked_array(Tracked* p);
```

| call | effect |
|---|---|
| `Tracked* p = make_tracked_array(3, 10);` | `p[0].value == 10`, `p[1].value == 11`, `p[2].value == 12`, `Tracked::alive == 3` |
| `free_tracked_array(p);` | `Tracked::alive == 0`, `Tracked::destroyed == 3` |
| `free_tracked_array(make_tracked_array(0, 5));` | `Tracked::alive == 0`, no crash |

`std::size_t` (from `<cstddef>`) is the unsigned integer type C++ uses for sizes and indices. Your loop counter should be one too; converting it to `int` for `value` wants a `static_cast<int>(i)`.
