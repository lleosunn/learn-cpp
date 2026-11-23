# Parsing with std::optional

A function that "returns an int, or nothing if the input is bad" is `-> int | None` in Python. C++17 spells it `std::optional<int>`, and the type system makes the "or nothing" part impossible to forget.

## The lesson

**`std::optional<T>`** is a `T` plus a flag saying whether it is present. The value is stored inline (no allocation) and the API is small:

```cpp
#include <optional>

std::optional<int> half(int n) {
    if (n % 2 != 0) return std::nullopt;   // "nothing"
    return n / 2;                          // an int converts to optional<int>
}

auto r = half(10);
if (r) { use(*r); }                        // bool test, then dereference
if (r.has_value()) { use(r.value()); }     // same, spelled out
int x = r.value_or(-1);                    // the value, or a fallback
if (auto h = half(7)) { use(*h); }         // declare and test in one line
```

Why not return a sentinel like `-1`? Then `-1` can never be a legitimate result, and nothing stops a caller from using the value without checking. Why not throw? Because a malformed line is an *expected* outcome for a parser, not a failure, and exceptions hide that control flow from the call site. `optional` puts the possibility of absence in the return type, so the caller must go through `*`, `.value()`, or `value_or` to reach the value.

One sharp edge: `*opt` on an empty optional is undefined behavior, and the sanitizers do not reliably catch it. `.value()` throws `std::bad_optional_access` instead. Prefer `.value()` or `value_or` unless you have just checked.

**Why not `std::stoi`?** It skips leading whitespace, stops at the first non-digit (`stoi("12a") == 12`), and *throws* on overflow. A strict parser walks the string itself: optional sign, at least one digit, nothing else. Accumulate in a `long long` and reject as soon as the running value passes the `int` limit; checking only at the end would overflow even `long long` on a 20-digit input, and signed overflow is undefined behavior that UBSan will flag.

**Structured bindings** unpack pairs, tuples, and plain structs:

```cpp
std::pair<std::string, int> kv{"port", 80};
auto [key, val] = kv;                        // copies; auto& [key, val] = kv; aliases
for (const auto& [k, v] : some_map) { ... }  // map entries are pairs
```

Combine them with `optional` and the `if`-with-initializer form and a config loop reads almost like Python:

```cpp
if (auto kv = parse_kv(line)) {   // kv is optional<pair<string, int>>
    auto [k, v] = *kv;
    cfg[k] = v;
}
```

## Coming from Python

- `return None` is `return std::nullopt;`. `if x is not None` is `if (x)` or `if (x.has_value())`.
- `x if x is not None else default` is `x.value_or(default)`.
- Python's `int("12a")` raises; C++'s `stoi("12a")` returns 12. Neither is "strict", hence the hand-written loop.
- `k, v = pair` is `auto [k, v] = pair;`. `for k, v in d.items()` is `for (const auto& [k, v] : m)`.
- `int` is 32-bit here: `"2147483647"` parses, `"2147483648"` does not. `"-2147483648"` is the smallest `int` and must parse.

## Your task

```cpp
std::optional<int> parse_int(const std::string& s);
// strict: optional leading '-', then one or more digits, nothing else; must fit in an int

std::optional<std::pair<std::string, int>> parse_kv(const std::string& line);
// "key=<int>": non-empty key before the first '=', parse_int on everything after it; no trimming

std::map<std::string, int> parse_config(const std::vector<std::string>& lines);
// parse_kv on each line, skip invalid lines, later lines override earlier keys
```

| call | result |
|---|---|
| `parse_int("42")` | `42` |
| `parse_int("-7")` | `-7` |
| `parse_int("+5")`, `parse_int("")`, `parse_int("12a")`, `parse_int(" 3")`, `parse_int("-")` | `nullopt` |
| `parse_int("2147483648")` | `nullopt` (does not fit) |
| `parse_kv("x=1")` | `("x", 1)` |
| `parse_kv("=1")`, `parse_kv("x")`, `parse_kv("x=y")`, `parse_kv("x=1=2")` | `nullopt` |
| `parse_config({"a=1", "bad", "b=2", "a=3"})` | `{a: 3, b: 2}` |
