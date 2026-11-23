# Value or Fallback

Chapter 2 gave you references: an alias that must refer to something. A pointer is the looser cousin. It holds an **address**, and that address is allowed to be "nothing".

## The lesson

`int x = 5;` puts a 4-byte integer somewhere in memory. `&x` is the address of that spot, and a variable that stores an address is a pointer:

```cpp
int x = 5;
const int* p = &x;   // p holds the address of x
int v = *p;          // *p reads the int that lives at that address: 5
```

The picture:

```text
     variable   contents
     --------   --------
p:   [ 0x7ff0 ] ----+        "p points to x"
                    |
x:   [ 5      ] <---+        (x lives at address 0x7ff0)
```

Two operators, two directions:

- `&x` is "address of": from a variable to its address.
- `*p` is "dereference": from an address to the value stored there.

`const int*` reads as "pointer to a const int": you can read through `p` but not write `*p = 6`. The pointer itself can still be reseated to a different int later.

The special value `nullptr` means "points at nothing":

```cpp
const int* q = nullptr;
int bad = *q;   // undefined behavior: there is no int at address 0
```

This is the "why". In Python, `None.value` raises `AttributeError` and you can catch it. In C++, dereferencing a null pointer is **undefined behavior**: the language promises nothing, not even a crash. The usual outcome is a segfault, but the compiler is also allowed to assume it never happens and optimize around it. In this course the sanitizer catches it and aborts with a report; in production you get whatever you get. The rule is simple: **check before you dereference.**

```cpp
if (p != nullptr) { /* safe to use *p */ }
if (p) { /* same thing: a pointer converts to bool, and null is false */ }
```

Note what `if (p)` tests: the *pointer*. `if (*p)` tests the *int it points at*, which is a different question, and is itself a dereference that must not happen on null.

## Coming from Python

- `nullptr` is the `None` of pointers. Forgetting the check is UB, not an exception. Nothing will catch it for you.
- `p ? *p : fallback` is C++'s conditional expression: Python's `*p if p else fallback`.
- A Python name always refers to some object. A C++ pointer can refer to nothing, and the type system does not force you to check. `std::optional` (Chapter 8) is the type that does.
- `*` does double duty: in a declaration (`const int* p`) it means "this is a pointer"; in an expression (`*p`) it means "read through it".

## Your task

Return the int `p` points at, or `fallback` if `p` is null.

```cpp
int value_or(const int* p, int fallback);
```

| call | result |
|---|---|
| `value_or(&x, -1)` with `x = 5` | `5` |
| `value_or(nullptr, -1)` | `-1` |
| `value_or(&z, -1)` with `z = 0` | `0` (the pointee is zero; the pointer is not null) |
| `value_or(&y, 0)` with `y = 9` | `9` |

The starter is deliberately wrong: it dereferences without checking. Run it first. The non-null cases pass, then the `nullptr` case aborts with a UBSan report ("load of null pointer"). That report is what an unchecked pointer looks like when you are lucky enough to have a sanitizer attached.
