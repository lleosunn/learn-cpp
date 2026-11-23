# Tray with std::move

A `unique_ptr` cannot be copied. So how does an object get from your hand into a container, and back out again? You *move* it. The syntax for that is the single most important thing in this chapter.

## The lesson

Every expression in C++ is either an **lvalue** (it has a name; you could take its address; it still exists after this line) or an **rvalue** (a temporary, about to die). Overload resolution tells them apart:

```cpp
std::vector<std::unique_ptr<Tracked>> items;
auto p = std::make_unique<Tracked>(1);

items.push_back(p);             // error: p is an lvalue -> push_back(const T&) -> tries to COPY -> deleted
items.push_back(std::move(p));  // ok: std::move casts p to an rvalue -> push_back(T&&) -> move constructor
// p is now empty (nullptr). The vector owns Tracked(1).
```

`std::move` does not move anything. It is a cast: "treat this lvalue as a temporary; I promise I no longer need its contents." The real work happens in the **move constructor** that the cast selects. For `unique_ptr`, that constructor copies the raw pointer into the new owner and sets the old one to `nullptr`. Cheap, and no `Tracked` is ever copied.

```text
before:   p ---> Tracked(1)        items: [ ]
after:    p = nullptr              items: [ ---> Tracked(1) ]
```

Two rules follow:

1. **After `std::move(x)`, do not read `x`.** A moved-from `unique_ptr` is null; dereferencing it is a null-pointer crash (UBSan will tell you). Assigning something new to `x` is fine.
2. **Returning a local by value is already a move.** `return out;` where `out` is a local `unique_ptr` moves it, or elides the copy entirely. `return std::move(out);` is redundant and actually *blocks* the elision; clang warns with `-Wpessimizing-move`.

Why so much ceremony around copying a pointer? Because the compiler has to know who is responsible for the `delete`. If two `unique_ptr`s silently held the same pointer, both destructors would run `delete` and you would be back to the double-free of chapter 4. Making the transfer explicit (`std::move`) and the copy impossible (`= delete`) is how the type system guarantees exactly one `delete`.

Taking a parameter *by value* is how a function says "give me ownership":

```cpp
void put(std::unique_ptr<Tracked> item);   // caller writes put(std::move(p)) or put(std::make_unique<Tracked>(3))
```

Inside `put`, `item` is a named local, therefore an lvalue again, so storing it needs another `std::move`. If you forget and `put` just returns, `item`'s destructor deletes the object at the closing brace. The starter does exactly that.

## Coming from Python

- `tray.put(obj)` in Python adds a second reference; both names see the same object forever. `tray.put(std::move(p))` hands the object over, and `p` is empty afterwards.
- There is no equivalent of `std::move` in Python, because there is no concept of a sole owner. The closest mental model is `item = p; p = None` done atomically, with the compiler checking that you never do it by accident.
- Returning a value is free in both languages, for different reasons: Python returns a reference; C++ moves or elides the object.

## Your task

`Tracked` is already defined for you:

```cpp
struct Tracked {
    int value;
    static inline int alive, constructed, destroyed, copies, moves;
    static void reset();
    Tracked(int v = 0);
};
```

Implement a last-in-first-out tray of owned `Tracked` objects:

```cpp
class Tray {
public:
    void put(std::unique_ptr<Tracked> item);   // take ownership of item
    std::unique_ptr<Tracked> take();           // hand back the most recently put item; nullptr if empty
    std::size_t size() const;
private:
    std::vector<std::unique_ptr<Tracked>> items_;
};
```

| sequence | result |
|---|---|
| `put(1); put(2); put(3)` | `size() == 3`, `Tracked::alive == 3` |
| `auto t = tray.take()` | `t->value == 3`, `size() == 2`, `alive == 3` (now owned by `t`) |
| `t.reset()` | `alive == 2` |
| `take()` on an empty tray | `nullptr` |
| tray destroyed | `alive == 0` |

The starter compiles. Run it first: `put` silently destroys every item at the end of the call, so `alive` comes out `0` where the tests expect `3`. The tests also check `copies == 0 && moves == 0` for `Tracked`: only pointers move, never the objects.
