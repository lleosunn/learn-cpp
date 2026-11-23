# Remove, Dedupe, Intersect

Three Python one-liners: `[x for x in v if x != t]`, `list(dict.fromkeys(v))`, `sorted(set(a) & set(b))`. The C++ versions are short too, but the first one is where every newcomer meets **iterator invalidation**.

## The lesson

**The starter does not compile. Run it first** and read the error: `no matching member function for call to 'erase'`. The code is

```cpp
for (int y : v)
    if (y == x) v.erase(/* ??? */);
```

and there is nothing to put in the `???`. A range-for gives you each element's value, not its position, and `erase` needs an iterator. The deeper problem is that even with an iterator, erasing from a vector while iterating over it is wrong: `erase` shifts every later element one slot left, so the iterator the loop holds now points at the element *after* the one it should visit next. Every iterator at or past the erased position is **invalidated**, and using one is undefined behavior.

The index-loop version compiles and is still wrong:

```cpp
for (std::size_t i = 0; i < v.size(); ++i)
    if (v[i] == x) v.erase(v.begin() + i);   // then ++i skips the element that slid into slot i
```

On `{1, 1, 2}` with `x = 1`: erase index 0, the second `1` slides into index 0, `++i` moves to index 1, the second `1` is never examined. Result `{1, 2}`. The test for this case is `remove_all({1,1,2}, 1)`.

The manual loop that *is* correct uses the iterator that `erase` returns, which points at the element that slid into the hole:

```cpp
for (auto it = v.begin(); it != v.end(); )
    if (*it == x) it = v.erase(it);   // erase returns the next valid iterator
    else ++it;
```

That is O(n²) because every erase shifts the tail. The idiom C++ programmers actually write is **erase-remove**:

```cpp
v.erase(std::remove(v.begin(), v.end(), x), v.end());
```

`std::remove` does not remove anything. It is an algorithm, so it cannot change the container's size; it only sees a range. It shifts every element that is *not* `x` to the front, preserving order, and returns an iterator to the new logical end. Everything from that iterator to `v.end()` is leftover junk. `v.erase(first, last)` then chops the junk off in one shot. Forget the `erase` and the vector still has its old size with garbage at the back.

**Membership with `unordered_set`.** `std::unordered_set<int>` is Python's `set`: O(1) `insert`, `count`, `erase`. `insert(y)` returns a `std::pair<iterator, bool>`; the `bool` is `true` if `y` was newly inserted, which is a one-step "have I seen this?" test. Iteration order is unspecified, so never return an `unordered_set`'s contents in order; here it is only used for membership.

**`std::set_intersection`.** Takes two **sorted** ranges and writes their common elements, in order, to an output iterator. The cheapest way to get sorted, unique input is to pour each vector into a `std::set<int>` (a sorted tree). `std::back_inserter(out)` is an output iterator that calls `out.push_back` for each write, so the result vector grows to fit.

| Python | C++ |
|---|---|
| `v = [y for y in v if y != x]` | `v.erase(std::remove(v.begin(), v.end(), x), v.end());` |
| `v = [y for y in v if not p(y)]` | `v.erase(std::remove_if(v.begin(), v.end(), p), v.end());` |
| `seen = set(); seen.add(y)` | `std::unordered_set<int> seen; seen.insert(y);` |
| `y in seen` | `seen.count(y)` |
| `sorted(set(a) & set(b))` | `std::set<int> sa(...), sb(...); std::set_intersection(...)` |

## Coming from Python

- Python lets you build a new list while reading the old one, so you never think about this. C++ `erase` mutates in place and moves memory; any iterator, pointer, or reference into the moved region is dead.
- `std::remove` is badly named. It is "partition the keepers to the front", not "delete". Always pair it with `erase`.
- C++20 adds `std::erase(v, x)` that does both. You are on C++17; write the idiom.
- `std::set` is sorted (a tree), `std::unordered_set` is hashed (like Python's). Pick `set` when you need order, `unordered_set` when you only need membership.
- `std::set_intersection` is a sorted-merge algorithm, O(n + m). Feeding it unsorted input gives wrong answers silently.

## Your task

```cpp
void remove_all(std::vector<int>& v, int x);
std::vector<int> dedupe_keep_order(const std::vector<int>& v);
std::vector<int> sorted_intersection(const std::vector<int>& a, const std::vector<int>& b);
```

- `remove_all` removes every occurrence of `x` from `v` in place, keeping the order of the rest.
- `dedupe_keep_order` returns the distinct values of `v` in order of first appearance.
- `sorted_intersection` returns the values present in both `a` and `b`, ascending, with no duplicates (set semantics). Inputs are not necessarily sorted.

| call | result |
|---|---|
| `remove_all({1, 2, 1, 3}, 1)` | `{2, 3}` |
| `remove_all({1, 1, 2}, 1)` | `{2}` |
| `remove_all({7, 7}, 7)` | `{}` |
| `dedupe_keep_order({3, 1, 3, 2, 1})` | `{3, 1, 2}` |
| `dedupe_keep_order({})` | `{}` |
| `sorted_intersection({1, 1, 2, 5}, {1, 3, 5, 5})` | `{1, 5}` |
| `sorted_intersection({1, 2}, {3, 4})` | `{}` |

Once you replace the body of `remove_all` the file compiles and the other two stubs fail their cases normally.
