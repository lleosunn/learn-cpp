# Sum and Max of a C Array

C arrays are the oldest container in the language and the one with the fewest safety rails. They explain why every C-style function that takes an array also takes a count.

## The lesson

```cpp
int a[3] = {4, 9, 2};
```

That is three ints, back to back, on the stack. `a[1]` is the middle one. So far it looks like a list.

```text
        a[0]    a[1]    a[2]    (not yours)
      +------+-------+-------+- - - - - -
a --> |   4  |   9   |   2   |   ???
      +------+-------+-------+- - - - - -
       arr    arr+1   arr+2   arr+3
```

The surprise is what happens when you pass it to a function. The array **decays**: `a` converts to an `int*` pointing at `a[0]`, and the length is gone. The function receives an address and nothing else.

```cpp
long long sum_array(const int* arr, std::size_t n);
sum_array(a, 3);   // a decays to &a[0]; the 3 must be passed by hand
```

That is the "why" behind the `n` parameter. Inside the function there is no `len(arr)`. `sizeof(arr)` is the size of a *pointer* (8 bytes), not the array. Clang warns about that (`-Wsizeof-array-argument`) only when the parameter is written with array syntax, `const int arr[]`; with `const int* arr` it stays silent. The size has to travel next to the pointer, and the function has to trust it.

Indexing is pointer arithmetic in disguise. `arr[i]` is defined as `*(arr + i)`, and `arr + i` means "advance by `i` ints" (the compiler scales by `sizeof(int)` for you). So `arr + 1` is the address of `a[1]`, and `&arr[1]` is the same address written differently. You need that when a function should return *which element*, not just its value.

Nothing stops you from writing `arr[3]` or `arr + 3`. The compiler cannot know `n`, so it cannot check. Reading `arr[n]` is undefined behavior; under the sanitizer it is a `stack-buffer-overflow` report, and without one it quietly reads whatever happens to sit next to the array. The classic way to get there is `i <= n`. Use `std::size_t` for the index, because that is the type of sizes and counts, and compare with `<`.

One more trap in the sum: `INT_MAX + INT_MAX` overflows `int`, which is also UB (UBSan reports it). Accumulate into a `long long`, and each `int` widens before it is added.

## Coming from Python

- There is no `len()`. The caller passes `n` and you believe them.
- `a[3]` on a 3-element list raises `IndexError`. Here it is undefined behavior; the sanitizer is your `IndexError`.
- `std::size_t` is unsigned. `i >= 0` is always true for it, so count up with `<`, not down to zero, until you know the idioms.
- Python's `max(a)` returns the value. `max_element_ptr` returns the *location*, so the caller can modify it or compute its index as `p - arr`. The STL's `std::max_element` (Chapter 6) works the same way.

## Your task

Two functions over a C array of `n` ints:

```cpp
long long sum_array(const int* arr, std::size_t n);
int* max_element_ptr(int* arr, std::size_t n);
```

`sum_array` returns the sum without overflowing. `max_element_ptr` returns a pointer to the **first** maximum element, or `nullptr` when `n == 0`.

| call | result |
|---|---|
| `sum_array(a, 3)` with `a = {1, 2, 3}` | `6` |
| `sum_array(nullptr, 0)` | `0` |
| `sum_array(big, 2)` with `big = {INT_MAX, INT_MAX}` | `4294967294` |
| `max_element_ptr(b, 3)` with `b = {4, 9, 2}` | `&b[1]` |
| `max_element_ptr(tie, 2)` with `tie = {5, 5}` | `&tie[0]` (first max) |
| `max_element_ptr(nullptr, 0)` | `nullptr` |

The test arrays are exact-size stack arrays. If you read one past the end, ASan will tell you, with the line number.
