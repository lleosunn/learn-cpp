# Tree with Weak Parent Links

Sometimes an object genuinely has more than one owner: a tree node is kept alive by its parent, but a caller may be holding it too. `shared_ptr` handles that with a reference count. `weak_ptr` is how you point at a shared object *without* keeping it alive, and it is the only thing standing between you and a leak.

## The lesson

`std::shared_ptr<T>` is a pointer plus a shared counter. Copying it bumps the count, destroying a copy decrements it, and whichever copy brings the count to zero deletes the object.

```cpp
auto a = std::make_shared<Node>();  // count 1
{
    auto b = a;                     // count 2: copying is allowed and cheap
    a.use_count();                  // 2
}                                   // b dies: count 1
// a dies: count 0 -> delete
```

`make_shared` allocates the object and its counter in one block; prefer it over `shared_ptr<T>(new T)`.

**The cycle problem.** Reference counting has one blind spot. Suppose a parent owns its children with `shared_ptr` and each child owns its parent with `shared_ptr` too:

```text
caller ---> root                 count(root)  = 1 (caller) + 1 (child->parent) = 2
             | children[0]
             v
            child                count(child) = 1 (root->children)
             | parent
             +---> root
```

The caller drops `root`: its count goes 2 → 1, never 0. `child`'s count never changes. Nothing is freed, ever. No sanitizer catches this on macOS; the `Tracked::alive` counter in the tests does.

**`weak_ptr` breaks the cycle.** A `std::weak_ptr<T>` points at a shared object but does not count. You cannot dereference it directly; you ask it for a temporary owner:

```cpp
std::weak_ptr<Node> parent = some_shared_ptr;   // no count change
if (auto p = parent.lock()) {                   // lock() -> shared_ptr, or empty if the object is gone
    p->tag.value;                               // safe: p keeps it alive for this block
}
```

`lock()` returns a *new* `shared_ptr`, so the count goes up by one while that `shared_ptr` exists. Observed from inside a traversal, `use_count()` is one higher than you might expect; assert counts from outside.

Rule of thumb: in an ownership diagram, arrows pointing *down* are `shared_ptr` (or `unique_ptr` if there is only one); back-pointers, caches and observers are `weak_ptr`. If you cannot draw the arrows without a loop, you have a leak.

## Coming from Python

- CPython also refcounts, and also cannot free a cycle by counting. It has a second collector that periodically walks the heap looking for unreachable cycles. C++ has nothing like it: a `shared_ptr` cycle is a leak until the process exits.
- `weakref.ref(obj)()` returns the object or `None`; `weak.lock()` returns a `shared_ptr` or an empty one. Same idea, except in C++ the result is a real owner for as long as you hold it.
- `sys.getrefcount(x)` and `x.use_count()` are both off by one for the same reason: the thing you are inspecting with is itself a reference.

## Your task

`Tracked` is already defined for you (`Tracked::alive` counts live instances). Each `Node` carries one, so a leaked tree shows up as `alive != 0` after the owning scope ends.

```cpp
struct Node {
    Tracked tag;                                    // tag.value is the node's value
    std::vector<std::shared_ptr<Node>> children;    // owns the children
    std::weak_ptr<Node> parent;                     // watches the parent
};

std::shared_ptr<Node> make_node(int value);                                       // a new root
std::shared_ptr<Node> add_child(const std::shared_ptr<Node>& parent, int value);  // new child, linked both ways
int depth(const std::shared_ptr<Node>& n);                                        // root -> 0, its child -> 1, ...
std::shared_ptr<Node> parent_of(const std::shared_ptr<Node>& n);                  // nullptr if n is a root or its parent is gone
```

| sequence | result |
|---|---|
| `root = make_node(1); c = add_child(root, 2); gc = add_child(c, 3)` | `depth(gc) == 2`, `Tracked::alive == 3` |
| `root.use_count()` | `1`: children do not own their parent |
| `c.use_count()` | `2`: the caller's `c` plus `root->children[0]` |
| all three leave scope | `alive == 0` |
| keep `c`, let `root` go | `parent_of(c) == nullptr`, `alive == 2` |

The starter declares `parent` as a `std::shared_ptr<Node>` on purpose. Implement the four functions against it first and run the tests: `root.use_count()` comes out `2`, and the "alive after scope" cases report three objects nobody can reach. Then change the member type and fix the two places that read it.
