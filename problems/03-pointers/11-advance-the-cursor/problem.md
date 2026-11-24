# Read a Number, Advance the Cursor

The chapter's two hardest ideas in one function: walking a C string, and changing the caller's pointer through a pointer to it. This is how real parsers are built.

## The lesson

A parser reads a string in pieces. Each piece needs to answer two questions: *what did I read?* and *where did I stop?*, so the next piece knows where to pick up. One of them can be the return value. The other goes back through an output parameter, as in problem 5, and here the thing being updated is the caller's position in the string: a `const char*`. Its address is a `const char**`.

```cpp
const char* cur = "12,34";
int n = read_number(&cur);   // n == 12, and cur now points at the ','
```

```text
              cur (before)    cur (after)
                 v               v
               +---+---+---+---+---+----+
               | 1 | 2 | , | 3 | 4 | \0 |
               +---+---+---+---+---+----+
```

Three levels, each with a different job:

| expression | type | what it is |
|---|---|---|
| `cursor` | `const char**` | your local copy of the address of the caller's `cur` |
| `*cursor` | `const char*` | the caller's `cur`: a position in the string |
| `**cursor` | `const char` | the character at that position |

Moving the caller's position means changing `*cursor`. This is where operator precedence bites:

| you write | what it does |
|---|---|
| `*cursor++` | postfix `++` binds tighter than `*`. It steps your **local** `cursor` to the "next `const char*`" (there is not one) and the caller's pointer does not move |
| `(*cursor)++` or `++*cursor` | steps the caller's pointer forward one character |

You can write the whole function with `**cursor` and `++*cursor`, and it is worth trying once. The cleaner idiom is **copy in, work, copy out**:

```cpp
const char* p = *cursor;   // a local copy of the caller's position
// ... read with *p, advance with ++p, exactly as in problem 4 ...
*cursor = p;               // write the final position back, once
```

All the double-star business shrinks to the first line and the last. In between, it is an ordinary string walk.

For the number itself: the digit characters `'0'` to `'9'` have consecutive codes, so `c - '0'` is the digit's value (the same char arithmetic as Chapter 1's letter shift). Reading left to right, each new digit shifts what you have one decimal place: `value = value * 10 + digit`.

The standard library does this too: `std::strtol(const char* s, char** end, int base)` returns the number and writes where it stopped into `*end`.

## Coming from Python

- Python would return both: `value, pos = read_number(s, pos)`. C-style code returns one and writes the other through a pointer.
- `int("12ab")` raises `ValueError`. This function reads as many digits as there are, stops, and tells you where. "No digits" is not an error here: it returns 0 and leaves the cursor alone, and the caller detects it by seeing that the cursor did not move.
- `c - '0'` is `ord(c) - ord('0')`.
- `for part in s.split(",")` is what a caller builds from this: read a number, check for a comma, step over it, repeat.

## Your task

```cpp
int read_number(const char** cursor);
```

Read the run of decimal digits that starts at `*cursor` and return its value. Leave `*cursor` pointing at the first character that is not a digit (possibly the terminator). If there is no digit at `*cursor`, return `0` and leave `*cursor` unchanged. `cursor` and `*cursor` are never null, and the values always fit in an `int`. No sign, no whitespace skipping.

| setup | call | returns | `cur` afterward |
|---|---|---|---|
| `cur = s`, `s = "12ab"` | `read_number(&cur)` | `12` | `s + 2` (on the `'a'`) |
| `cur = s`, `s = "907"` | `read_number(&cur)` | `907` | `s + 3` (on the terminator) |
| `cur = s`, `s = "007x"` | `read_number(&cur)` | `7` | `s + 3` |
| `cur = s`, `s = "abc"` | `read_number(&cur)` | `0` | `s` (unchanged) |
| `cur = s + 1`, `s = "12ab"` | `read_number(&cur)` | `2` | `s + 2` |

The last group of tests reads `"3,14,159"` the way a caller would: call, check for the comma, `++cur`, call again. Open the **Tests** tab to see it.
