# ScopedTracked

Three problems in, you have written `delete` by hand every time and the tests had to check that you remembered. This problem makes the compiler remember for you. It is the single most important idea in C++.

## The lesson

Manual cleanup fails at the first function with more than one way out:

```cpp
int process(int v) {
    Tracked* t = new Tracked(v);
    if (v < 0) return -1;          // leaked t
    if (!validate(*t)) return -2;  // leaked t again
    int r = compute(*t);
    delete t;                      // only reached on the happy path
    return r;
}
```

Every `return` needs its own `delete`, and the next person to add a `return` forgets. Exceptions make it worse: any call can leave the function without reaching the next line.

C++ gives you one guarantee that solves all of it: **a local object's destructor runs when its scope ends, no matter how the scope ends.** Normal fall-through, early `return`, `break`, `continue`, a thrown exception: the destructor runs, in reverse order of construction. You cannot forget it because you never write the call.

So put the `delete` in a destructor:

```cpp
class Owner {
public:
    explicit Owner(int v) : p_(new Tracked(v)) {}  // acquire
    ~Owner() { delete p_; }                        // release: automatic, every path
private:
    Tracked* p_;
};

int process(int v) {
    Owner o(v);                 // from here on, cleanup is the compiler's job
    if (v < 0) return -1;       // ~Owner runs
    // ...
}                               // ~Owner runs
```

This pattern is called **RAII**, Resource Acquisition Is Initialization. The resource (heap memory here, but equally a file, a lock, a socket) is acquired in the constructor and tied to the object's lifetime. `~Owner()` is the destructor: no parameters, no return type, called by the compiler at the closing brace.

Two details complete the pattern:

- **Forbid copying.** The compiler writes a copy constructor for you that copies `p_` bit for bit. `Owner b = a;` would give two wrappers one object, and both destructors would delete it: a double free. `Owner(const Owner&) = delete;` turns that line into a compile error instead. Chapter 5 teaches the alternative of writing a real copy.
- **A way out.** Sometimes the owner must hand the object to someone else. `release()` returns the raw pointer and sets `p_` to `nullptr`, so the destructor has nothing to delete (`delete nullptr` is a no-op) and the caller is now the owner.

`explicit` on the constructor stops `ScopedTracked s = 7;` from compiling; a one-argument constructor without it would silently convert any `int` into a wrapper.

## Coming from Python

- `with open(...) as f:` runs `__exit__` at the end of the block, but only when the caller remembers to write `with`. In C++ every local object gets this behavior, every time, with no special syntax at the call site.
- `__del__` exists but you were told not to rely on it: it runs whenever the collector gets around to it, and possibly never for reference cycles. A C++ destructor runs at a known line, deterministically.
- `try/finally` is the manual version of RAII. RAII is `finally` written once in the class instead of in every caller.

## Your task

`Tracked` is already defined for you (`value`, counters `alive`, `constructed`, `destroyed`, `copies`, `moves`, and `reset()`). The tests use its counters because Apple's clang has no LeakSanitizer.

Complete `ScopedTracked`. The constructor already allocates. Make the destructor release, make copying a compile error, and implement `release()` so the wrapper gives up ownership and the caller becomes responsible for the `delete`.

```cpp
class ScopedTracked {
public:
    explicit ScopedTracked(int v);
    ~ScopedTracked();
    ScopedTracked(const ScopedTracked&) = delete;
    ScopedTracked& operator=(const ScopedTracked&) = delete;
    Tracked& get();
    const Tracked& get() const;
    Tracked* release();   // hand back the pointer; the wrapper then owns nothing
private:
    Tracked* p_;
};
```

| code | effect |
|---|---|
| `{ ScopedTracked s(7); /* alive == 1, s.get().value == 7 */ }` | after `}`: `Tracked::alive == 0` |
| `{ ScopedTracked s(3); raw = s.release(); }` | after `}`: `Tracked::alive == 1`; the test then runs `delete raw;` |
| `ScopedTracked b = a;` | does not compile |

A destructor that deletes even after `release()` will double-free in the release test; read the AddressSanitizer report the same way as in problem 3.
