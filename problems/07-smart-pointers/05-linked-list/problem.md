# Singly Linked List with unique_ptr

The last problem made you write five special member functions. This one makes you write zero. If every member of a class owns its resource through a type that already knows how to copy, move and destroy itself, the compiler-generated operations are correct, and the best code is the code you never wrote.

## The lesson

A linked-list node owns the next node. Say that in the type:

```cpp
struct Node {
    int value;
    std::unique_ptr<Node> next;   // this node owns its successor
};
std::unique_ptr<Node> head_;      // the list owns the first node
```

Now the ownership graph *is* the data structure:

```text
head_ ---> [1 | next] ---> [2 | next] ---> [3 | next] ---> nullptr
```

Destroying `head_` deletes node 1, whose destructor destroys its `next`, which deletes node 2, and so on. No destructor to write. Copying a `List` is a compile error (a `unique_ptr` member deletes the copy constructor), and moving one is a cheap `noexcept` pointer handoff, because `unique_ptr`'s move is. That is the **rule of zero**: declare none of destructor/copy/move; let the members handle it.

**Traversing without owning.** Walk the list with a raw `Node*`. Here a raw pointer means "I am looking, not owning", and it is the right tool:

```cpp
for (Node* cur = head_.get(); cur; cur = cur->next.get()) { /* cur->value */ }
```

Do not write `auto n = head_;` (a copy; will not compile) or `auto n = std::move(head_);` while walking (you just detached the whole list from the object).

**Re-linking is moving.** Inserting at the front is three moves:

```cpp
auto n = std::make_unique<Node>();
n->value = v;
n->next = std::move(head_);   // n now owns the old chain
head_ = std::move(n);         // the list now owns n
```

Popping the front is one: `head_ = std::move(head_->next);`. The right-hand side releases node 2 from node 1 first; then `head_` lets go of node 1, which is deleted, and node 1's `next` is already empty, so nothing else goes with it.

`reverse()` is the same idea in a loop: peel the head off, point it at the reversed prefix, repeat. Draw it before you type it.

**One real limit.** Destruction is recursive: `~Node` → `~unique_ptr<Node>` → `~Node` → …, one stack frame per node. A thousand nodes is fine; around a million the stack overflows. The fix is a destructor that unlinks iteratively:

```cpp
~List() { while (head_) head_ = std::move(head_->next); }
```

But the moment you declare that destructor, the compiler stops generating the moves (rule of five, again) and you must add `List(List&&) noexcept = default;` and `List& operator=(List&&) noexcept = default;` back. The tests stay at a thousand nodes so you can keep the rule of zero here; know where the cliff is.

## Coming from Python

- `class Node: next = None` plus the GC gives you this structure for free, and a cycle (`a.next = b; b.next = a`) is still collected. Here a cycle cannot even be expressed: a `unique_ptr` chain is acyclic by construction.
- `b = a` on a Python list aliases. `List b = a;` here is a compile error, which is the honest answer when a class owns heap memory; `List b = std::move(a);` is the explicit transfer.
- There is no equivalent of `std::move`, and no equivalent of the rule of zero either, because Python never asked you to write `__del__` or `__copy__` for ordinary classes. C++ asks only when you hold a raw resource; hold it through `unique_ptr` and you are back to writing nothing.

## Your task

```cpp
class List {
public:
    void push_front(int v);
    void push_back(int v);
    bool pop_front(int& out);         // false if empty (out untouched); otherwise out = first value
    std::size_t size() const;
    std::vector<int> to_vector() const;
    void reverse();
private:
    struct Node {
        int value;
        std::unique_ptr<Node> next;
    };
    std::unique_ptr<Node> head_;
    std::size_t size_ = 0;
};
```

Do **not** add a destructor or copy/move operations: the last tests check that `List` is non-copyable and nothrow-movable, which the rule of zero gives you for free.

| sequence | result |
|---|---|
| `push_front(3); push_front(2); push_front(1)` | `to_vector() == {1, 2, 3}` |
| `push_back(1); push_back(2); push_back(3)` | `to_vector() == {1, 2, 3}` |
| `pop_front(x)` three times on `{1, 2, 3}`, then once more | `1, 2, 3`, then `false` |
| `reverse()` on `{1, 2, 3}`, on `{}`, on `{5}` | `{3, 2, 1}`, `{}`, `{5}` |
| 1000 × `push_front` | `size() == 1000`, `to_vector()` ascending |
