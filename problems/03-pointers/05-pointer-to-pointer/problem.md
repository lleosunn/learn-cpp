# Find and Hand Back a Pointer

Everything so far changed an `int` through a pointer. This problem changes the caller's *pointer* through a pointer to it. Same rule, one more star.

## The lesson

Recall the swap bug: assigning to a parameter changes your local copy. That holds for pointer parameters too. If a function receives `int* out` and does `out = &arr[1];`, it has re-pointed its own copy, and the caller's `int*` is exactly where it was.

To change a variable from inside a function you need its address. The caller's variable is an `int*`, so its address is an `int**`:

```cpp
int* found = nullptr;
find_first(a, 4, 3, &found);   // &found is an int**
```

```text
caller's frame                          find_first's frame
a:     [ 4 | 3 | 3 | 1 ]
found: [ nullptr ]  <-----------------  out: [ &found ]

after `*out = arr + 1;` inside find_first:
found: [ &a[1] ] --> a[1]
```

Inside the function:

- `out` is the address of `found`.
- `*out` is `found` itself, an `int*`. Writing `*out = arr + i;` sets the caller's pointer.
- `**out` is the int `found` points at, once it points somewhere.

The type system helps you here. `out = &arr[i];` does not compile: `out` is an `int**` and `&arr[i]` is an `int*`. In problem 2 the same mistake compiled and silently did nothing; with two levels of indirection it becomes a type error, which is the best kind of bug.

Two details separate a correct implementation from a plausible one:

1. **`out` may be null.** The contract lets the caller pass `nullptr` to say "I only want the bool". Writing `*out = ...` through a null `out` is a store to address 0 (UBSan reports "store to null pointer"). Check `out` before every write, including the reset.
2. **Reset `*out` on the not-found path.** If the caller's `found` held a pointer from a previous call and you return `false` without touching `*out`, the caller now has `false` plus a stale pointer that still looks valid. Set `*out = nullptr` first, then search. An output parameter should always leave the caller in a known state.

This is the "why" of output parameters: when a function must return two things (found or not, and where), C-style code returns one and writes the other through a pointer the caller supplies. You will see `T**` and `T*&` in C APIs and older C++ constantly. Chapter 8's `std::optional` is the modern replacement.

## Coming from Python

- There is no "variable that holds a variable" in Python. The nearest analogy is passing a one-element list and doing `box[0] = value` inside the function.
- `int**` is not a 2D array. It is a pointer to one `int*`. Read declarations from the right: "out is a pointer to a pointer to int".
- `*out = arr + i` and `*out = &arr[i]` are the same thing. Pick one and be consistent.
- Multiple return values are `return found, idx` in Python. In modern C++ you would return a struct or `std::optional<int*>`; here you practice the C-style version so you can read it when you meet it.

## Your task

Search `arr[0..n)` for `target` and return whether it was found. If `out` is not null, set `*out` to a pointer to the first match, or to `nullptr` if there is none.

```cpp
bool find_first(int* arr, std::size_t n, int target, int** out);
```

| call | result |
|---|---|
| `find_first(a, 4, 3, &out)` with `a = {4, 3, 3, 1}` | `true`; `out == &a[1]` |
| then `*out = 99` | `a[1] == 99` (the pointer is live) |
| `find_first(a, 4, 7, &out)` | `false`; `out == nullptr`, even if `out` was non-null before |
| `find_first(a, 4, 3, nullptr)` | `true`, no crash |
| `find_first(nullptr, 0, 3, &out)` | `false`; `out == nullptr` |

The starter returns `false` and never touches `out`. The **Tests** tab shows how `&out` is passed in and checked afterward.
