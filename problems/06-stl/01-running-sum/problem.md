# Prefix Sums and Safe Indexing

`std::vector` is the C++ list. It is also the first place a Python programmer silently corrupts memory, because `v[i]` does not check `i`.

## The lesson

A `std::vector<T>` is a growable array of `T`, stored contiguously on the heap. The API you need most:

| Python list | `std::vector<int> v` |
|---|---|
| `v = [1, 2, 3]` | `std::vector<int> v{1, 2, 3};` |
| `v.append(x)` | `v.push_back(x);` |
| `len(v)` | `v.size()` (returns `std::size_t`) |
| `v[i]` | `v[i]` (no check) or `v.at(i)` (throws if out of range) |
| `v[-1]` | `v.back()` |
| `v.pop()` | `v.pop_back();` (returns nothing) |
| `not v` | `v.empty()` |
| `v = []` | `v.clear();` |

Two things differ from Python in ways that matter.

**`v.size()` is unsigned.** Its type is `std::size_t`, a 64-bit unsigned integer. Index with `std::size_t`, not `int`: `for (std::size_t i = 0; i < v.size(); ++i)`. Mixing signed `int` with unsigned `size_t` in a comparison earns a `-Wsign-compare` warning, and for good reason. Unsigned arithmetic wraps: `v.size() - 1` on an empty vector is not `-1`, it is `18446744073709551615`, and a loop written as `i <= v.size() - 1` runs "forever" (until it reads past the end and crashes).

**`v[i]` does not bounds-check.** Python raises `IndexError`. C++ `operator[]` just computes an address and reads it. If `i` is past the end, that is undefined behavior: you might get garbage, you might read another object's memory, you might crash later somewhere unrelated. This tutorial compiles with AddressSanitizer, which turns that into an immediate, loud report; production code does not get that favor. `v.at(i)` does check and throws `std::out_of_range`, but idiomatic C++ checks the index itself and uses `[]`.

**`reserve` is an optimization, not a resize.** `push_back` doubles the buffer whenever it fills up. If you know the final size, `out.reserve(n)` allocates once up front. `reserve` does not change `size()`; `out[0]` is still out of range until you `push_back`.

```cpp
std::vector<int> out;
out.reserve(v.size());   // capacity n, size 0
for (int x : v) out.push_back(x * 2);
```

Range-for over a vector reads `for (int x : v)` (a copy of each element) or `for (int& x : v)` (a reference, so you can modify in place).

## Coming from Python

- There are no negative indices. `v[-1]` converts `-1` to a huge `size_t` and reads garbage.
- No slicing syntax. `std::vector<int>(v.begin() + 1, v.begin() + 3)` is `v[1:3]`.
- A vector holds one type. `std::vector<int>` cannot hold a string.
- Returning a vector by value is fine and fast. The compiler elides the copy.
- Brace-initialized vectors like `std::vector<int> v{1, 2, 3, 4}` allocate exactly 4 slots, so `v[4]` is immediately outside the allocation.

## Your task

Write two functions.

`running_sum` returns the prefix sums of `v`: element `i` of the result is `v[0] + ... + v[i]`. The input is not modified.

`get_at` is a bounds-checked read: if `i` is a valid index, store `v[i]` in `out` and return `true`; otherwise return `false` and leave `out` untouched.

```cpp
std::vector<int> running_sum(const std::vector<int>& v);
bool get_at(const std::vector<int>& v, std::size_t i, int& out);
```

| call | result |
|---|---|
| `running_sum({1, 2, 3, 4})` | `{1, 3, 6, 10}` |
| `running_sum({})` | `{}` |
| `running_sum({-1, 1})` | `{-1, 0}` |
| `get_at({1, 2, 3, 4}, 2, out)` | `true`, `out == 3` |
| `get_at({1, 2, 3, 4}, 4, out)` | `false` |
| `get_at({}, 0, out)` | `false` |

**Run the starter first.** Its `get_at` is the unchecked `out = v[i]; return true;`. The `running_sum` cases fail normally, `get_at(v, 2)` passes, and then `get_at(v, v.size())` produces an AddressSanitizer report: `heap-buffer-overflow ... READ of size 4` right past the end of a 16-byte allocation. That is the vector's buffer, and the read is `v[4]`. Everything after that case never runs. Fix the bounds check and the report disappears.
