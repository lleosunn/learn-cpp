# Longest Word by Reference

You know `const T&` avoids a copy on the way *in*. It can also avoid a copy on the way *out*, but only when the thing you refer to is still alive after the function returns.

## The lesson

A function can return a reference:

```cpp
const std::string& first(const std::vector<std::string>& v) {
    return v[0];   // refers into the caller's vector
}
```

No string is copied. `v[0]` lives inside the caller's vector, which existed before the call and still exists after it, so the reference the caller gets back is valid.

Now the broken version:

```cpp
const std::string& first_copy(const std::vector<std::string>& v) {
    std::string s = v[0];   // a local: lives in this function's stack frame
    return s;               // a reference to s...
}                           // ...and s is destroyed right here
```

Locals die at the closing brace of the function. The returned reference now points at memory that has been released and will be reused by the next function call. Reading through it is **undefined behavior**: it may appear to work, return garbage, or crash, depending on what has happened to that stack memory since. Clang warns (`-Wreturn-stack-address`) and AddressSanitizer catches it at the first read (`stack-use-after-return`). Outside this tutorial, with optimizations on and no sanitizer, nobody catches it for you.

The rule: **a returned reference must refer to something that outlives the call.** Safe targets: an element of a parameter that was itself passed by reference, a member of the object you are a method of (chapter 5), a `static`, a global. Never a local, never a by-value parameter, never a temporary.

The caller has a matching obligation:

```cpp
const std::string& w = longest_word(make_words());   // DANGLING
```

`make_words()` returns a temporary vector that is destroyed at the end of the full statement, at the semicolon. `w` now refers into a dead vector. Same bug, other side of the call. When the source is a temporary, take a copy: `std::string w = longest_word(make_words());`.

Practical technique for "return a reference to the best element": do not hold a copy of the best element while you search. Hold an *index* (or an iterator, chapter 6) and return `words[best]` at the end. Whatever you return must be an expression that names something inside `words`.

Why return references at all? For large elements, `const T&` is free and `T` is a copy. `std::vector::operator[]`, `std::map::at`, `std::max_element` (through an iterator) all hand you references into the container for exactly this reason. Knowing when that is safe is not optional.

## Coming from Python

- Returning a local in Python is always fine; the object lives as long as anything references it. A C++ stack object dies at `}` no matter who still holds a reference, and the reference keeps pointing at the corpse.
- Python's `max(words, key=len)` returns the very object that sits in the list. The C++ version returns a *reference* to that object, which is what the `&longest_word(v) == &v[1]` check proves.
- "First wins ties" means compare with `>` not `>=`, the same tie rule Python's `max` uses.

## Your task

Return a reference to the longest string in `words`. You may assume `words` is non-empty. If several strings share the maximum length, return the first. Do not copy the string.

```cpp
const std::string& longest_word(const std::vector<std::string>& words);
```

| call | result |
|---|---|
| `longest_word({"a", "bbb", "cc"})` | `"bbb"` |
| `longest_word({"aa", "bb"})` | `"aa"` |
| `longest_word({"x"})` | `"x"` |
| `longest_word({"", "abc", "abcd", "ab"})` | `"abcd"` |
| `&longest_word(v) == &v[1]` for `v = {"a", "bbb", "cc"}` | `true` |

The starter is deliberately wrong: it copies `words[0]` into a local and returns a reference to that local. It compiles, with a warning. Run it first: read the warning, then read the AddressSanitizer report that follows. That report is what this bug looks like in the wild on a lucky day.
