# Seconds → H:MM:SS

Python strings are one type that does everything. C++ has two things that look like strings in source code and behave nothing alike, and a good part of week one is learning not to mix them up.

## The lesson

**String literals are not `std::string`.** `"abc"` is a `const char[4]`: four bytes in read-only memory (`a`, `b`, `c`, and a terminating zero). Passed around, it decays to a `const char*`, a pointer to the first byte. It has no `.size()`, no `+`, and no `==` that compares contents. (Chapter 3 goes deep on this. For now: a literal is raw bytes, not an object.)

`std::string` from `<string>` is the real string class: it owns its characters, grows on demand, and has the operators you expect.

```cpp
#include <string>

std::string s = "abc";       // literal -> std::string: the conversion happens here
s += "def";                  // append a literal: fine, s is a std::string
s += 'g';                    // append one char
std::string t = s + "!";     // + works as long as ONE side is a std::string
std::string u = "a" + "b";   // compile error: const char* + const char*
std::string v = "0" + 5;     // compiles (clang warns): pointer arithmetic, 5 bytes past "0"
```

The last line is the one that hurts. `"0" + 5` is a pointer into memory you do not own; reading through it is a bug ASan reports as a global-buffer-overflow, and without ASan it prints garbage or crashes. Build a `std::string` first, then concatenate.

**Numbers to text.** There are no f-strings in C++17 (C++20's `std::format` comes close, but it is not available in this build). `std::to_string(42)` gives `"42"`. Zero-padding you write yourself; `std::ostringstream` with `std::setw` also works but is more machinery than this problem needs.

**Helper functions.** C++ reads a file top to bottom, and a function must be *declared* before the line that calls it. Define `pad2` above `format_hms` or the compiler says `use of undeclared identifier 'pad2'`. Python resolves names when the call runs; C++ resolves them at compile time, in order. (In a multi-file program you would mark a file-local helper `static` so it cannot collide with another file's; one file, one function, no need.)

**`const` locals.** `const int h = total_seconds / 3600;` says this value is computed once and never reassigned. The compiler enforces it and the reader can stop tracking it. Default to `const` for anything you do not intend to change; it is free documentation.

## Coming from Python

- `f"{h}:{m:02}:{s:02}"` becomes `std::to_string(h) + ":" + pad2(m) + ":" + pad2(s)`. Verbose, but every piece is a `std::string` operation you can see.
- `"a" + "b"` is a compile error. `"a" "b"` (adjacent literals, no operator) is legal and means `"ab"`; it is how long literals are split across lines.
- `str(n)` is `std::to_string(n)`; `int(s)` is `std::stoi(s)`.
- Python strings are immutable; `std::string` is mutable. `s += "x"` appends in place.
- `'0'` and `"0"` are different types (`char` and `const char[2]`). `'0' + 5` is the `int` 53, which happens to be the code for `'5'`: the char-is-a-number rule from the previous problem.

## Your task

Format a non-negative number of seconds as `H:MM:SS`: hours with no padding (they may exceed 23), minutes and seconds always two digits.

```cpp
std::string format_hms(int total_seconds);
```

| call | result |
|---|---|
| `format_hms(0)` | `"0:00:00"` |
| `format_hms(59)` | `"0:00:59"` |
| `format_hms(3723)` | `"1:02:03"` |
| `format_hms(86399)` | `"23:59:59"` |
| `format_hms(90000)` | `"25:00:00"` |

`total_seconds` is always `>= 0`. The starter returns an empty string, so every case fails until you fill it in.
