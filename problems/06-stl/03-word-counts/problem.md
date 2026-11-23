# Word Frequency and Top-K

`collections.Counter(words).most_common(k)` in two functions. Along the way: `std::map`, the `operator[]` that inserts, `std::pair`, and sorting with a custom comparator.

## The lesson

C++ has two dictionaries. `std::map<K, V>` is a balanced tree: iteration is always sorted by key, lookups are O(log n). `std::unordered_map<K, V>` is a hash table like Python's `dict`: O(1) lookups, but iteration order is unspecified and changes between runs and compilers. Python's `dict` remembers insertion order; neither C++ map does. When output order matters, use `std::map` or sort explicitly.

| Python `d` | C++ `std::map<std::string, int> m` |
|---|---|
| `d[k] = v` | `m[k] = v;` |
| `d[k]` (KeyError if absent) | `m.at(k)` (throws if absent) |
| `d.get(k, 0)` | `auto it = m.find(k); it == m.end() ? 0 : it->second` |
| `k in d` | `m.count(k)` or `m.find(k) != m.end()` |
| `d[k] += 1` (needs defaultdict) | `++m[k];` (inserts 0 first) |
| `for k, v in d.items():` | `for (const auto& [k, v] : m)` |
| `del d[k]` | `m.erase(k);` |
| `len(d)` | `m.size()` |

**`operator[]` inserts.** `m[k]` on a missing key creates it with a value-initialized `V` (`0` for `int`, `""` for `std::string`) and returns a reference. That is why `++counts[w]` is the whole counting loop, with no `setdefault` dance. It is also why `if (m["zzz"] > 0)` silently grows the map by one entry. To look without inserting, use `find`, `count`, or `at`.

**A `const` map has no `operator[]`.** Since `[]` might insert, it is not available on a `const std::map&`. Writing `counts[w]` inside `top_k` is a compile error, not a runtime surprise. Use `find` or iterate.

**Iterating gives `std::pair<const K, V>`.** Each element is a pair; `.first` is the key, `.second` the value. C++17 structured bindings make it readable: `for (const auto& [word, n] : counts)`. 

**Sorting by value means copying out.** A map cannot be reordered. Copy the pairs into a `std::vector<std::pair<std::string, int>>` and `std::sort` it with a comparator. A comparator is a function (usually a lambda) that returns `true` when its first argument must come before the second:

```cpp
std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) {
    if (a.second != b.second) return a.second > b.second;  // count, descending
    return a.first < b.first;                              // tie: word, ascending
});
```

That is Python's `sorted(items, key=lambda kv: (-kv[1], kv[0]))`, written as a two-argument "comes before" test instead of a key. The comparator must be a strict ordering: never return `true` for `(a, a)`. Using `>=` or `<=` breaks that rule and `std::sort` may crash.

**Truncating a vector.** `v.resize(n)` with `n < v.size()` drops the tail. Clamp first: `v.resize(std::min(k, v.size()))`, since resizing *up* would pad with empty pairs.

## Coming from Python

- No insertion order. `std::map` iterates in key order; `std::unordered_map` in no useful order. Tests that depend on `unordered_map` order are flaky by construction.
- `pair.first` / `pair.second` are the C++ spelling of a 2-tuple. `std::make_pair("a", 1)` or brace-init `{"a", 1}` builds one.
- `std::sort` sorts in place and returns nothing. There is no `sorted()` that returns a copy; copy first, then sort.
- `std::min(k, v.size())` requires both arguments to have the same type. `k` is `std::size_t` here for exactly that reason.

## Your task

```cpp
std::map<std::string, int> word_counts(const std::vector<std::string>& words);
std::vector<std::pair<std::string, int>> top_k(const std::map<std::string, int>& counts, std::size_t k);
```

`word_counts` returns how many times each word appears. `top_k` returns up to `k` `(word, count)` pairs ordered by count descending, ties broken by word ascending. If `k` exceeds the number of distinct words, return all of them.

| call | result |
|---|---|
| `word_counts({"b", "a", "b"})` | `{"a": 1, "b": 2}` |
| `word_counts({})` | `{}` |
| `top_k({"a": 2, "b": 2, "c": 1}, 2)` | `{("a", 2), ("b", 2)}` |
| `top_k({"a": 2, "b": 2, "c": 1}, 10)` | `{("a", 2), ("b", 2), ("c", 1)}` |
| `top_k({"a": 2, "b": 2, "c": 1}, 0)` | `{}` |

The starter compiles and returns empty containers. The test output prints maps as `{"a": 1, "b": 2}` and vectors of pairs as `[("a", 2), ("b", 2)]`, so ordering mistakes are easy to spot.
