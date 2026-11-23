# strlen and strchr by Hand

A C string is a `char` array with a sentinel instead of a length. Walking one is pointer arithmetic in its purest form, and `const char*` is the type you will meet at every C API boundary.

## The lesson

```cpp
const char* s = "hello";
```

The literal `"hello"` is a `const char[6]`: five letters plus a terminating `'\0'` (the NUL character, byte value 0). It decays to a pointer like the int array did, so `s` is the address of `'h'` and nothing more. No length is stored anywhere. The only way to find the end is to walk forward until you hit the zero byte.

```text
s --> +---+---+---+---+---+----+
      | h | e | l | l | o | \0 |
      +---+---+---+---+---+----+
       s   s+1 s+2 s+3 s+4 s+5
```

Pointer arithmetic on a `const char*`:

- `*s` is the character under the pointer.
- `++s` moves to the next character.
- `s + 2` is the address of the first `'l'`; `s[2]` is the same as `*(s + 2)`.
- `p - s` is the number of elements between two pointers into the same array. Its type is `std::ptrdiff_t` (signed). For a length, cast it to `std::size_t`.

A length function is "walk to the terminator, subtract":

```cpp
const char* p = s;
while (*p != '\0') ++p;   // '\0' is 0, so `while (*p)` is the same test
// p - s is the length
```

The "why": the terminator is the *only* thing standing between your loop and the memory past the string. If `find_char` walks forward looking for `'z'` and never checks for `'\0'`, it runs off the end of the literal. Under ASan that is a `global-buffer-overflow` report (string literals live in global memory). Without a sanitizer it keeps going until it happens to find a `'z'` byte or an unmapped page. Every loop over a C string needs its stop condition before its step.

Three places `const` can sit, and they mean different things:

| declaration | what is frozen |
|---|---|
| `const char* p` | the chars (`*p = 'x'` is an error); `p` can still move |
| `char* const p` | the pointer (`++p` is an error); the chars are writable |
| `const char* const p` | both |

Read right to left: `const char* p` is "p is a pointer to a char that is const". Parameters that only read a string take `const char*`, which also lets callers pass string literals.

## Coming from Python

- `len("hello")` is O(1). `strlen` is O(n) because it walks. Do not call it inside a loop condition.
- `"hello" == "hello"` in Python compares contents. `s == "hello"` in C++ compares two *addresses*, which is almost never what you want. Compare C strings with `std::strcmp`, or convert to `std::string` first.
- A Python `str` can contain `'\0'`. A C string ends at the first one: `"ab\0cd"` has length 2 as far as `strlen` can tell.
- `std::string` (Chapter 1) carries its own length and is what you should normally use. `const char*` is what you get from `argv`, `.c_str()`, and every C library.

## Your task

Reimplement two functions from `<cstring>` without calling them:

```cpp
std::size_t c_strlen(const char* s);
const char* find_char(const char* s, char c);
```

`c_strlen` returns the number of characters before the terminator. `find_char` returns a pointer to the first occurrence of `c` in `s`, or `nullptr` if it is not there. Like `strchr`, searching for `'\0'` finds the terminator.

| call | result |
|---|---|
| `c_strlen("hello")` | `5` |
| `c_strlen("")` | `0` |
| `c_strlen("ab\0cd")` | `2` |
| `find_char(s, 'l')` with `s = "hello"` | `s + 2` |
| `find_char(s, 'z')` | `nullptr` |
| `find_char(s, '\0')` | `s + 5` |
| `find_char("", 'a')` | `nullptr` |

The tests check pointer identity (`== s + 2`), not the characters, so return a pointer *into* `s`, not a copy.
