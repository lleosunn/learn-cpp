# Compare Two C Strings

Problem 4 warned that `==` on two `const char*` compares addresses. This is the practice round for doing it properly: two pointers walking two different arrays in lockstep.

## The lesson

```cpp
char x[] = "cat";
char y[] = "cat";
x == y;   // false: two different arrays at two different addresses
```

`x` and `y` hold the same letters, but `==` on pointers asks "are these the same place in memory?", and they are not. Comparing the *contents* means walking both strings one position at a time:

```text
a --> | c | a | t | \0 |
b --> | c | a | r | \0 |
        =   =   x          first difference at index 2: not equal
```

At each position there are three cases, and the order you test them in matters:

1. `*a != *b`: the strings differ. Stop, `false`.
2. They are equal *and* `*a == '\0'`: both strings ended at the same position. Stop, `true`.
3. They are equal and not the terminator: step both pointers.

Notice that one terminator check is enough. By the time you reach case 2 you already know `*a == *b`, so if one is `'\0'` the other is too. And a shorter string needs no special handling: its `'\0'` lines up against a real character in the longer one, which is just case 1.

The trap is the short version that looks right:

```cpp
while (*a == *b) { ++a; ++b; }   // BUG
return false;
```

For two equal strings the terminators are equal as well, so this loop steps past both of them and keeps comparing whatever memory comes next. Under ASan that is a `stack-buffer-overflow` report. Every C string loop needs a way to stop *at* the terminator.

`starts_with` is the same walk with a different stop: you are done when `prefix` runs out. If `s` runs out first, its `'\0'` sits against a real prefix character, they differ, and you return `false` before stepping any further. That is the only thing keeping you inside `s`.

## Coming from Python

- Python's `a == b` on strings compares contents and `a is b` compares identity. C++ `==` on two `const char*` is Python's `is`.
- `s.startswith(prefix)` is `starts_with`. An empty prefix matches everything, in both languages.
- The real thing is `std::strcmp(a, b) == 0` from `<cstring>`. It returns an `int` (negative, zero, positive) so it can also sort; `if (strcmp(a, b))` is true when the strings are *different*, a classic source of bugs.
- `std::string` overloads `==` to compare contents. This problem is why that exists.

## Your task

Without `<cstring>`:

```cpp
bool c_streq(const char* a, const char* b);
bool starts_with(const char* s, const char* prefix);
```

`c_streq` returns whether the two strings have the same length and the same characters. `starts_with` returns whether `s` begins with all of `prefix`. Both are case-sensitive.

| call | result |
|---|---|
| `c_streq(x, y)`, two separate buffers both holding `"hello"` | `true` |
| `c_streq("hello", "help!")` | `false` |
| `c_streq("hello", "hell")` | `false` (one is a prefix of the other) |
| `c_streq("", "")` | `true` |
| `starts_with("hello", "he")` | `true` |
| `starts_with("hello", "")` | `true` |
| `starts_with("he", "hello")` | `false` (the prefix is longer than the string) |
| `starts_with("hello", "el")` | `false` (present, but not at the start) |

The starter's `c_streq` is deliberately wrong: it is `return a == b;`. Run it first. It passes when both arguments are the same buffer and fails on two buffers with the same letters. The tests use exact-size `char` arrays, so walking past a terminator is an ASan report.
