# Make and Destroy One Object

Everything you have built so far lived on the stack and died at the closing brace. This is your first object that outlives the function that created it, and your first debt to repay.

## The lesson

Local variables live in the current function's **stack frame**. The frame is torn down when the function returns, and everything in it goes with it:

```cpp
Tracked* bad() {
    Tracked t(7);   // lives in bad()'s frame
    return &t;      // address of something about to vanish
}                   // t destroyed here; the returned pointer now dangles
```

To make something that survives the return, ask for memory from the **heap** (also called the free store):

```cpp
Tracked* good() {
    return new Tracked(7);   // constructed on the heap; nobody destroys it for you
}
```

`new T(args)` does two things: it allocates space for one `T` on the heap and runs `T`'s constructor there. It hands back a `T*`. That object stays alive until someone runs `delete` on that exact pointer, which runs the destructor and gives the memory back.

```text
      stack (automatic)                    heap (manual)
   +--------------------+              +------------------+
   | main's frame       |              |                  |
   |   Tracked* p  -----+------------->|  Tracked{7}      |  <- new Tracked(7)
   +--------------------+              |                  |
   | good()'s frame     |  gone at }   +------------------+
   +--------------------+              still here after }
```

Rules that follow:

- One `new` pairs with exactly one `delete`. Zero deletes is a **leak**: the object stays alive until the process exits. Two deletes is a **double free**: undefined behavior, and AddressSanitizer aborts the program when it sees one.
- `delete nullptr;` is defined to do nothing. You never need `if (p) delete p;`.
- After `delete p;` the variable `p` still holds the old address. Reading through it is a **use-after-free**. If `p` lives on, the habit is `p = nullptr;` right after.

Why this matters: the pointer `make_tracked` returns is a promise. Whoever holds it is now responsible for deleting it. That responsibility is called **ownership**, and deciding who owns what is most of what makes C++ different from Python. The rest of this chapter is about moving that responsibility somewhere the compiler enforces it.

If a test aborts with an AddressSanitizer report, the first stack trace is where the bad access happened; look for the first line that mentions `solution.cpp`.

## Coming from Python

- In Python every object is on the heap and the garbage collector frees it when the last reference disappears. In C++ you choose: locals are on the stack and vanish at `}`; `new` puts an object on the heap and it vanishes only when you say so.
- `Tracked(7)` in Python allocates. In C++, `Tracked t(7);` is a stack object and `new Tracked(7)` is a heap object. The syntax difference is the whole point.
- There is no `del`. Python's `del p` drops a reference; C++'s `delete p` destroys the object right now, whatever else still points at it.

## Your task

`Tracked` is already defined for you. It counts its own lifetime events, so the tests can prove you did not leak without a leak detector (Apple's clang ships no LeakSanitizer):

```cpp
struct Tracked {
    int value;
    static int alive;        // constructed minus destroyed
    static int constructed;  // every constructor call
    static int destroyed;    // every destructor call
    static int copies;       // copy constructor + copy assignment
    static int moves;        // move constructor + move assignment
    static void reset();     // zero all counters
    Tracked(int v = 0);
};
```

Write two functions. `make_tracked` returns a new heap `Tracked` whose `value` is `v`. `destroy_tracked` destroys an object returned by `make_tracked`; it must be safe to call with `nullptr`.

```cpp
Tracked* make_tracked(int v);
void destroy_tracked(Tracked* p);
```

| call | effect |
|---|---|
| `Tracked* p = make_tracked(7);` | `Tracked::alive == 1`, `p->value == 7` |
| `destroy_tracked(p);` | `Tracked::alive == 0`, `Tracked::destroyed == 1` |
| `destroy_tracked(nullptr);` | nothing happens |

The tests read `Tracked::alive` after each call. A missing `delete` shows up as `alive == 1` where `0` was expected.
