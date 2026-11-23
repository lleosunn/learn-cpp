# Sort, Filter, Reduce a Roster

`sorted(key=...)`, `filter`, `sum`, `next(...)`. C++ has them all in `<algorithm>` and `<numeric>`, but they take iterator pairs instead of sequences, and the sort takes a "comes before" test instead of a key.

## The lesson

Every STL algorithm works on a half-open range `[first, last)`: `first` points at the first element, `last` points one past the end. For a vector that is `v.begin()` and `v.end()`. Algorithms never see the container, only the range, which is why they work on vectors, strings, arrays, and halves of vectors alike.

| Python | C++ |
|---|---|
| `v.sort(key=f)` | `std::sort(v.begin(), v.end(), cmp)` |
| `next((x for x in v if p(x)), None)` | `std::find_if(v.begin(), v.end(), p)` → iterator or `v.end()` |
| `sum(f(x) for x in v)` | `std::accumulate(v.begin(), v.end(), init, op)` |
| `[x for x in v if p(x)]` | `std::copy_if(v.begin(), v.end(), std::back_inserter(out), p)` |
| `[f(x) for x in v]` | `std::transform(v.begin(), v.end(), std::back_inserter(out), f)` |
| `any(p(x) for x in v)` | `std::any_of(v.begin(), v.end(), p)` |
| `v.count(x)` | `std::count(v.begin(), v.end(), x)` |

**Comparators, not keys.** `std::sort` takes a function `cmp(a, b)` that returns `true` when `a` must come before `b`. For "score descending, then name ascending":

```cpp
std::sort(v.begin(), v.end(), [](const Student& a, const Student& b) {
    if (a.score != b.score) return a.score > b.score;
    return a.name < b.name;
});
```

The comparator must be a **strict weak ordering**: `cmp(a, a)` is `false`, and if `cmp(a, b)` then not `cmp(b, a)`. Writing `a.score >= b.score` breaks both rules on ties. That is undefined behavior; with many equal elements `std::sort` can read past the end of the array and crash. One of the tests sorts 1000 tied students for exactly this reason.

**Lambdas.** `[](const Student& a, const Student& b) { ... }` is an anonymous function. The `[]` is the capture list: `[]` captures nothing, `[&]` lets the body use local variables of the enclosing function by reference (needed when the predicate depends on a parameter like `name`).

**`accumulate` and the seed type.** `std::accumulate(first, last, init, op)` computes `op(op(op(init, x0), x1), x2)...` and the result has the type of `init`. Pass `0` and the whole sum is an `int`; `250 / 3` is `83`, not `83.33`. Pass `0.0`.

**From iterator to pointer.** `find_if` returns an iterator. `*it` is the element itself (a reference into the vector), so `&*it` is its address. Returning `const Student*` lets the caller have `nullptr` for "not found" and a direct view into the vector otherwise, with no copy. That pointer is valid until the vector reallocates or the element is erased.

**Filtering into a different type.** `copy_if` copies whole elements; when you want a different type (names from students) a plain loop with `if` and `push_back` is the clearest tool. Not every loop needs to become an algorithm.

## Coming from Python

- `std::sort` is in place and returns `void`. No `sorted()` copy.
- No `key=`. Convert "sort by (-score, name)" into "a before b iff ...".
- Algorithms take `v.begin(), v.end()`, never `v`. Forgetting `.end()` is a long compile error.
- Returning a pointer is how C++ says `Optional[Student]` without a copy. Check for `nullptr` before dereferencing.
- Integer division strikes again in `mean_score`. The seed decides.

## Your task

`Student` is already defined in the starter; do not change it.

```cpp
struct Student { std::string name; int score; };

void sort_students(std::vector<Student>& v);                                   // score desc, then name asc
std::vector<std::string> names_above(const std::vector<Student>& v, int min_score); // score > min_score, input order
double mean_score(const std::vector<Student>& v);                               // 0.0 if empty
const Student* find_student(const std::vector<Student>& v, const std::string& name); // nullptr if absent
```

- `sort_students` reorders `v` in place: higher score first; equal scores alphabetical by name.
- `names_above` returns the names of students whose score is strictly greater than `min_score`, in the order they appear in `v`.
- `mean_score` returns the arithmetic mean as a `double`, `0.0` for an empty roster.
- `find_student` returns a pointer to the element *inside* `v` with that name (the first one if duplicated), or `nullptr`.

With `v = {{"bob", 80}, {"amy", 90}, {"cat", 80}}`:

| call | result |
|---|---|
| `sort_students(v)` then names | `{"amy", "bob", "cat"}` |
| `names_above(v, 85)` | `{"amy"}` |
| `names_above(v, 79)` | `{"bob", "amy", "cat"}` (input order) |
| `mean_score(v)` | `83.333...` |
| `mean_score({})` | `0.0` |
| `find_student(v, "cat")` | `&v[2]` |
| `find_student(v, "dan")` | `nullptr` |

The starter compiles: `sort_students` does nothing, the others return empty, `0.0`, or `nullptr`.
