# make_unique and Scope

In chapter 4 you wrote `new`/`delete` by hand and then wrapped them in a class so the destructor would clean up. The standard library already ships that class. It is called `std::unique_ptr`, and from here on you will almost never write `delete` again.

## The lesson

`std::unique_ptr<T>` is a tiny object that holds a `T*` and calls `delete` on it in its destructor. That is the whole trick: ownership lives in a stack object with a destructor, so cleanup is automatic on every exit path (`return`, exception, `break`).

```cpp
#include <memory>

{
    std::unique_ptr<Tracked> p = std::make_unique<Tracked>(5);  // new Tracked(5), owned by p
    p->value;        // -> works like a raw pointer
    (*p).value;      // so does *
    p.get();         // the raw Tracked*, non-owning, for handing to old APIs
}                    // p's destructor runs here: delete happens, nothing to remember
```

What makes it "unique":

- **One owner.** Copying is a compile error (`unique_ptr(const unique_ptr&) = delete`). The next problem covers how ownership moves between owners.
- **It can be empty.** A default-constructed `unique_ptr` holds `nullptr`; `if (p)` tests that. Dereferencing an empty one crashes exactly like a null raw pointer.
- **`reset` and assignment free the old object.** `p.reset()` deletes now and leaves `p` empty. `p = std::make_unique<T>(...)` deletes the old object, then takes the new one.
- **Zero overhead.** `sizeof(std::unique_ptr<T>) == sizeof(T*)`, and `->` compiles to a plain pointer access.

Why `make_unique` instead of `new`? `std::unique_ptr<T> p(new T(...))` works, but the moment `new` appears in your code you are holding a raw pointer for however many lines pass before the wrapper takes it. If anything throws in between, it leaks. `make_unique` allocates and wraps in a single expression, so there is no window. Rule: if you type `new`, stop and ask why.

```text
stack                      heap
+-------------------+      +--------------+
| p : unique_ptr    | ---> | Tracked{5}   |
+-------------------+      +--------------+
     p leaves scope  -->  delete runs  -->  heap object gone
```

How do you *read* an object someone else owns? Take a reference. `int value_of(const std::unique_ptr<Tracked>& p)` borrows the pointer and leaves ownership alone. `void f(std::unique_ptr<Tracked> p)` by value would demand that the caller hand ownership over (next problem). And if a function does not care who owns the object, the most honest signature is `int value_of(const Tracked& t)`, called as `value_of(*p)`.

## Coming from Python

- Python's GC owns every object; references are free and the object dies "eventually". `unique_ptr` is the opposite: exactly one owner, destroyed at a deterministic line, no collector running.
- `p = make_unique(...)` on a `unique_ptr` looks like rebinding a name, but the old object is freed *on that line*, not whenever a refcount happens to hit zero.
- `p.get()` gives a raw pointer that does not keep the object alive. It is closer to a `weakref` than to an ordinary Python reference.

## Your task

`Tracked` is already defined for you: an `int` holder that counts its own lifetime events, so the tests can prove you freed what you allocated and never copied the object itself.

```cpp
struct Tracked {
    int value;
    static inline int alive, constructed, destroyed, copies, moves;
    static void reset();
    Tracked(int v = 0);
};
```

Write three functions:

```cpp
std::unique_ptr<Tracked> make_tracked(int v);             // a new Tracked holding v
int value_of(const std::unique_ptr<Tracked>& p);          // p->value, or -1 if p is empty
void replace_with(std::unique_ptr<Tracked>& p, int v);    // p now owns a new Tracked(v); the old one is destroyed
```

| call | result |
|---|---|
| `auto p = make_tracked(5); value_of(p)` | `5`, and `Tracked::alive == 1` until `p` leaves scope |
| `value_of(std::unique_ptr<Tracked>{})` | `-1` |
| `replace_with(p, 9)` | `p->value == 9`, `Tracked::destroyed == 1`, `Tracked::alive == 1` |

The tests also check `Tracked::copies == 0` and `Tracked::moves == 0`: a `unique_ptr` moves the *pointer*, never the object it points to. If a copy shows up, you wrote something like `Tracked t = *p;`.
