# Split, Join, Upper

Python's `str` ships with `split`, `join`, and `upper`. `std::string` ships with `find`, `substr`, and a `char` you can loop over. Writing the three Python methods once is the fastest way to learn the C++ string API.

## The lesson

`std::string` is a container of `char`, with the same shape as `std::vector<char>` plus text-specific helpers. Treat it like a vector you can also search:

| Python | C++ `std::string s` |
|---|---|
| `len(s)` | `s.size()` |
| `s[i]` | `s[i]` (a `char`, no check) |
| `s += "x"` | `s += "x";` or `s.push_back('x');` |
| `s.find("ab")` → `-1` if absent | `s.find("ab")` → `std::string::npos` if absent |
| `s[2:5]` | `s.substr(2, 3)` (start, **length**) |
| `s[2:]` | `s.substr(2)` |
| `for ch in s:` | `for (char c : s)` or `for (char& c : s)` to mutate |
| `s == t` | `s == t` |
| `"a" + "b"` | `std::string("a") + "b"` (at least one side must be a `std::string`) |

**`find` and `npos`.** `s.find(x, start)` returns the index of the first `x` at or after `start`, or the constant `std::string::npos` if none. `npos` is `std::size_t(-1)`, the largest possible `size_t`. It is not `-1`: `if (s.find(',') < 0)` is always false and compiles with a warning at best. Compare against `npos` by name.

```cpp
std::size_t pos = s.find(',');
if (pos == std::string::npos) { /* not found */ }
```

**Splitting is a cursor loop.** Keep a `start` index, find the next delimiter from there, copy `[start, pos)` out with `substr`, move `start` past the delimiter. When `find` says `npos`, the piece from `start` to the end is the last one, and it is pushed even when empty. That last push is what makes `split("", ',')` return `{""}` and `split(",", ',')` return `{"", ""}`, exactly like Python's `"".split(",")` and `",".split(",")`.

**Characters are small integers.** `char` is a 1-byte integer type, and `<cctype>` functions like `std::toupper` take an `int` that must be in the range of `unsigned char` (0 to 255). On most platforms plain `char` is signed, so a byte like `0xE9` (part of "é" in UTF-8) is `-23`. Passing a negative to `std::toupper` is undefined behavior. The idiom is to cast through `unsigned char` on the way in and back to `char` on the way out:

```cpp
c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
```

**Pass by value when you want a copy.** `std::string to_upper(std::string s)` takes `s` by value, so the function already owns a scratch copy. Mutate it in place and return it. No `const&` plus manual copy needed.

## Coming from Python

- Strings are mutable. `s[0] = 'X'` and `s += "..."` modify `s` in place; there is no string interning to worry about.
- `substr` takes a start and a *length*, not an end index. `s.substr(a, b - a)` is `s[a:b]`.
- `'a'` is a `char`, `"a"` is a string literal. `split(s, ',')` takes a `char`; `join(parts, ", ")` takes a `std::string`.
- Building a string with repeated `+=` is linear time (amortized), unlike naive concatenation in some languages. It is the normal way to build output.
- There is no `str.upper()` that handles Unicode. `std::toupper` works on single bytes in the current locale; for ASCII it does what you expect.

## Your task

Write three functions that behave like Python's `s.split(delim)`, `sep.join(parts)`, and `s.upper()` (ASCII only).

```cpp
std::vector<std::string> split(const std::string& s, char delim);
std::string join(const std::vector<std::string>& parts, const std::string& sep);
std::string to_upper(std::string s);
```

`split` must match Python's `str.split(sep)` with an explicit separator: every delimiter produces a boundary, empty pieces are kept, and there is always at least one piece.

| call | result |
|---|---|
| `split("a,b,,c", ',')` | `{"a", "b", "", "c"}` |
| `split("", ',')` | `{""}` |
| `split(",", ',')` | `{"", ""}` |
| `split("abc", ',')` | `{"abc"}` |
| `join({"x", "y", "z"}, ", ")` | `"x, y, z"` |
| `join({}, ",")` | `""` |
| `to_upper("MiXed 1!")` | `"MIXED 1!"` |

`join(split(s, ','), ",")` must give back `s` for any `s`.

The starter compiles and returns empty results (and `to_upper` returns its input unchanged), so you will see which cases fail.
