# Half-Open Ranges

Practice with pointer arithmetic, and a preview: this is the shape every STL algorithm in Chapter 6 takes.

## The lesson

So far a function learned about an array through a pointer and a count. There is a second way to say the same thing: **two pointers**.

```cpp
int a[5] = {4, 7, 1, 7, 9};
const int* first = a;       // the first element
const int* last  = a + 5;   // one past the last element
```

```text
      first                         last
        v                             v
      +-----+-----+-----+-----+-----+- - - -
      |  4  |  7  |  1  |  7  |  9  |  ???
      +-----+-----+-----+-----+-----+- - - -
```

The pair is written `[first, last)`: `first` is included, `last` is not. That is why it is called half-open. `last` is the one-past-the-end pointer from the previous problem, so you compare against it and never dereference it.

Everything about the range falls out of the two pointers:

| question | answer |
|---|---|
| how many elements? | `last - first` |
| is it empty? | `first == last` |
| visit each one | `for (const int* p = first; p != last; ++p)` |
| the rest after position `p` | `[p + 1, last)` |

The loop has no index and no `n`. The pointer itself is the loop variable, `*p` is the current element, and `p != last` is the stop condition.

Now the convention that surprises people: when a search finds nothing, it returns **`last`**, not `nullptr`. `last` is already a pointer that no element can have, so it works as "not found" without inventing a second special value. It also composes. After a hit at `p`, the search for the next one is `find_in_range(p + 1, last, target)`, and when that returns `last` you are done.

```cpp
const int* hit = find_in_range(a, a + 5, 7);    // a + 1
if (hit != a + 5) { /* found: *hit is safe to read */ }
```

Sub-ranges cost nothing: `[a + 2, a + 5)` is the last three elements, with no copying.

## Coming from Python

- `range(2, 5)` and `a[2:5]` are half-open for the same reason: the length is `5 - 2` and adjacent ranges do not overlap.
- `a.index(x)` raises `ValueError` on a miss and `s.find(x)` returns `-1`. Here a miss returns `last`. Always compare the result against the `last` *you passed in* before dereferencing.
- A Python slice is a copy. A pointer pair is a view into the caller's array.
- In Chapter 6, `v.begin()` and `v.end()` are exactly `first` and `last`, and `std::find(v.begin(), v.end(), x)` returns `v.end()` on a miss.

## Your task

Two functions over the range `[first, last)`:

```cpp
const int* find_in_range(const int* first, const int* last, int target);
std::size_t count_in_range(const int* first, const int* last, int target);
```

`find_in_range` returns a pointer to the first element equal to `target`, or `last` if there is none. `count_in_range` returns how many elements equal `target`.

With `a = {4, 7, 1, 7, 9, 7}`:

| call | result |
|---|---|
| `find_in_range(a, a + 6, 7)` | `a + 1` (the first 7) |
| `find_in_range(a + 2, a + 6, 7)` | `a + 3` (the search starts after the first 7) |
| `find_in_range(a, a + 6, 5)` | `a + 6` (a miss returns `last`) |
| `find_in_range(a, a + 1, 7)` | `a + 1` (7 is outside the range, so this is `last`) |
| `find_in_range(a + 2, a + 2, 1)` | `a + 2` (empty range) |
| `count_in_range(a, a + 6, 7)` | `3` |
| `count_in_range(a, a, 4)` | `0` |
