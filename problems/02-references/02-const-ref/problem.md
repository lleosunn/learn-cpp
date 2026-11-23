# Sum Without Copying

Problem 1 showed that parameters are copies. For an `int`, who cares. For a `std::vector` holding a thousand objects, a copy is a heap allocation plus a thousand constructor calls, on every call. `const&` is how you opt out.

## The lesson

```cpp
int sum_values(std::vector<Tracked> items) { ... }   // copies the whole vector on every call
```

Calling this with a 1000-element vector allocates a new buffer, copy-constructs 1000 `Tracked`s, runs the loop, then destroys all 1000 copies on return. The function only wanted to *read* them.

The fix is a reference, as in problem 1, plus `const` to promise you will not modify:

```cpp
int sum_values(const std::vector<Tracked>& items) { ... }  // no copy, read-only
```

Read `const std::vector<Tracked>&` right to left: "a reference to a vector of Tracked that is const." Three things you get:

- No copy. `items` is an alias for the caller's vector.
- Compiler-enforced read-only. `items.push_back(...)` or `items[0].value = 5` inside the function is a compile error.
- Unlike a plain `T&`, a `const T&` can bind to temporaries, so `sum_values({1, 2, 3})` works.

This is the default way to pass anything bigger than a couple of machine words. `int`, `double`, `char`, `bool`, pointers: by value. `std::string`, `std::vector`, your own structs: `const&`.

The same trap hides inside range-for loops:

```cpp
for (Tracked t : items)         // t is a COPY of each element
for (const Tracked& t : items)  // t is a read-only alias of each element
for (const auto& t : items)     // same, with the type deduced
```

The by-value loop copy-constructs every element one at a time. It is the most common accidental copy in beginner C++, and `auto` without `&` does not save you: `for (auto t : items)` copies too.

Why you should care beyond speed: if the element type owns a resource (a file handle, a socket, a big buffer), copying it may be expensive, forbidden (non-copyable types exist), or subtly wrong (two objects now think they own the same thing). Getting into the `const&` habit now means those problems never show up later.

The tests catch copies with `Tracked`, a struct that counts its own constructor calls. After calling your function they check `Tracked::copies == 0`. That is how a copy that is invisible in the output becomes a failing test.

## Coming from Python

- Passing a list to a Python function costs nothing: one more reference to the same list. Passing a `std::vector` by value in C++ is a deep copy.
- Python has no `const`. Read-only-ness is convention (`tuple` instead of `list`, a docstring). In C++ it is checked by the compiler and is part of the function's signature.
- `for t in items` in Python binds `t` to each existing element. `for (Tracked t : items)` constructs a fresh copy each iteration. You need `&` to get Python's behavior.

## Your task

Return the sum of `.value` over all elements, without copying any `Tracked`.

```cpp
int sum_values(const std::vector<Tracked>& items);
```

`Tracked` is already defined for you:

```cpp
struct Tracked {
    int value;
    static int alive, constructed, destroyed, copies, moves;  // lifetime counters
    static void reset();                                      // zero every counter
    Tracked(int v = 0);
    // copy/move constructors and assignments bump `copies` / `moves`
};
```

| call | result | `Tracked::copies` after the call |
|---|---|---|
| `sum_values({1, 2, 3})` | `6` | `0` |
| `sum_values({})` | `0` | `0` |
| `sum_values({-5, 5})` | `0` | `0` |
| 1000 elements with values `1..1000` | `500500` | `0` |

The braces in the table stand for a vector built before the call, which is how the tests do it. A literal `sum_values({1, 2, 3})` would report 3 copies even with a perfect `sum_values`, because building the temporary vector copies each element out of the initializer list.

The starter compiles and returns the right numbers but makes 6 copies for a 3-element input. Run it first and see which checks fail.
