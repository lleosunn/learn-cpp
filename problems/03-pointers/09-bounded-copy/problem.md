# Copy into a Fixed Buffer

Until now every string was read-only. This practice round *writes* one through a `char*`, into a buffer whose size you are told and must respect.

## The lesson

A writable C string is a `char` array the caller owns:

```cpp
char buf[6];   // room for 5 characters plus the '\0'
```

Copying `"hello world"` into it has to stop early, and still leave a valid string behind:

```text
src --> | h | e | l | l | o |   | w | o | r | l | d | \0 |

dst --> | h | e | l | l | o | \0 |      cap = 6: five chars copied, then the terminator
          0   1   2   3   4   5         dst[6] is not yours
```

C's original `strcpy(dst, src)` takes no size at all. It copies until it reaches `src`'s terminator, however far away that is. If `src` is longer than the buffer, it keeps writing over whatever sits after it: other variables, return addresses, anything. That is the **buffer overflow**, the most exploited bug in the history of C.

The fix is the rule from problem 3: the size travels with the pointer. `cap` is the *total* number of bytes at `dst`, so there is room for at most `cap - 1` characters, because one byte belongs to the terminator. Three things to get right:

1. **Never write at or past `dst[cap]`.** The last index you own is `cap - 1`.
2. **Always terminate when `cap > 0`.** A `char` buffer without a `'\0'` is not a string. The next `c_strlen` on it walks off the end, far away from the code that caused it.
3. **`cap == 0` means write nothing**, not even the terminator. And mind `cap - 1`: `cap` is a `std::size_t`, so with `cap == 0` it wraps to a huge number. The condition `i + 1 < cap` says the same thing as `i < cap - 1` without subtracting.

Truncating silently is a bug waiting for a caller, so the function returns how many characters it actually copied. The caller can compare that with what it expected.

`dst[i] = src[i]` is `*(dst + i) = *(src + i)`: one read through a `const char*`, one write through a `char*`. The types are the contract. `src` is `const char*` because you only read it; `dst` is `char*` because you write it.

## Coming from Python

- Python strings are immutable and sized to fit. There is no "destination buffer"; `s[:5]` just makes a new string. The nearest thing to `buf` is `bytearray(6)`.
- Nothing here raises on overflow. Writing `dst[cap]` is undefined behavior; ASan reports it as a `stack-buffer-overflow`.
- Real code uses `std::snprintf(buf, sizeof buf, "%s", src)` for fixed buffers, and `std::string`, which grows on its own, for everything else.
- `sizeof buf` gives 6 where `buf` is declared. Inside `copy_string` the array has decayed to a pointer, which is why `cap` has to be passed.

## Your task

```cpp
std::size_t copy_string(char* dst, std::size_t cap, const char* src);
```

Copy `src` into `dst`, which has room for `cap` bytes in total. Copy at most `cap - 1` characters, then write the terminator. Return the number of characters copied, not counting the terminator. If `cap == 0`, write nothing and return `0`.

| call | returns | `dst` afterward |
|---|---|---|
| `copy_string(buf, 6, "hello")` | `5` | `"hello"` (exact fit) |
| `copy_string(buf, 6, "hello world")` | `5` | `"hello"` (truncated) |
| `copy_string(buf, 4, "hello")` | `3` | `"hel"` |
| `copy_string(buf, 8, "hi")` | `2` | `"hi"` |
| `copy_string(buf, 1, "hello")` | `0` | `""` (only the terminator fits) |
| `copy_string(buf, 6, "")` | `0` | `""` |
| `copy_string(nullptr, 0, "hello")` | `0` | untouched |

Each test buffer is exactly `cap` bytes and pre-filled with `'x'`, so a missing terminator shows up as trailing `x`s and one byte too many is an ASan report.
