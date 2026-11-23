# Min and Max via Out-Params

A C++ function returns one value. When you need two, or a result plus a success flag, one old and still-everywhere idiom is to write the results *into* reference parameters.

## The lesson

An **out-parameter** is a non-const reference the function writes to:

```cpp
bool parse_int(const std::string& s, int& out) {
    // ... try to parse s ...
    if (failed) return false;   // out untouched
    out = parsed;
    return true;
}

int n = -1;
if (parse_int("42", n)) {
    // n is 42
}
```

The pieces of the pattern:

- Inputs come first, by value or `const&`. Outputs come last, as plain `T&`.
- The return value carries the success flag. The caller checks it before trusting the outputs.
- **On failure, leave the outputs alone.** The caller may have set them to sentinels or to the previous good values. A function that half-writes its outputs and then returns `false` creates bugs that show up far from the cause.

That last rule has a direct consequence: check your preconditions *before* the first write. The obvious "set `lo` to the largest possible int and `hi` to the smallest, then loop over everything" is correct math, but it has already clobbered `lo` and `hi` by the time it discovers the vector is empty. Start from `v[0]` instead, after the empty check.

Why this idiom exists when `std::pair` and `std::optional` do too (chapters 6 and 8): it predates them, it needs no extra types, and it is baked into APIs you will meet daily. `std::getline(std::cin, line)` writes into `line` and returns something you test for success. `std::from_chars` writes into an out-param. Nearly every C and OS API works this way. New code often prefers returning a struct or `optional`, but you have to read and write out-params fluently either way.

Two small things the solution needs:

- `lo = hi = v[0];` assigns right to left, like Python's `lo = hi = v[0]`.
- Index with `std::size_t`, the unsigned type `v.size()` returns. Comparing a signed `int i` against `v.size()` produces a warning under `-Wall`, and the warning is right: mixing signed and unsigned comparisons is a classic bug source.

## Coming from Python

- Python returns `(lo, hi)` as a tuple, or `None` for an empty list. C++ can do that too (`std::optional<std::pair<int, int>>`, later), but this problem teaches the reference idiom because you will meet it constantly.
- Python's `min([])` raises `ValueError`. Out-param functions signal failure with the return value instead of an exception, and the caller is responsible for checking it.
- The caller must pass *variables* for `lo` and `hi`. `min_max(v, 0, 0)` does not compile: a literal is not something you can write into.

## Your task

Find the smallest and largest element of `v`. If `v` is non-empty, set `lo` and `hi` and return `true`. If `v` is empty, return `false` and leave `lo` and `hi` **exactly as they were**.

```cpp
bool min_max(const std::vector<int>& v, int& lo, int& hi);
```

| `v` | returns | `lo` after | `hi` after |
|---|---|---|---|
| `{3, 1, 2}` | `true` | `1` | `3` |
| `{7}` | `true` | `7` | `7` |
| `{-4, -9, 0}` | `true` | `-9` | `0` |
| `{5, 5, 5}` | `true` | `5` | `5` |
| `{}` | `false` | unchanged | unchanged |

The tests preset `lo` and `hi` to `42` before every call and check that the empty case leaves them at `42`. The starter returns `false` and never touches the outputs, so it fails everything except the empty case.
