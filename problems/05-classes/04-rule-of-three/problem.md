# IntBuffer Deep Copy

`IntBuffer` owns a heap array through a raw pointer. The compiler will happily copy it for you, and what it generates is wrong in a way that crashes. This problem is about telling the compiler what copying *means* for a class that owns something.

## The lesson

**What the compiler writes for you.** If you do not declare them, C++ generates three special member functions:

| member | invoked by | default behavior |
|---|---|---|
| destructor `~T()` | end of scope, `delete` | destroys each member |
| copy constructor `T(const T&)` | `T b = a;`, `T b(a);`, pass by value, return by value | copies each member |
| copy assignment `T& operator=(const T&)` | `b = a;` when `b` already exists | assigns each member |

"Copies each member" is memberwise, which for `int* data_` means copying the *pointer*. Now two objects point at one array. When the first one dies, its destructor runs `delete[] data_`. When the second dies, it deletes the same array again. That is a double free, and AddressSanitizer stops the program with `attempting double-free`. Before that, `b[0] = 99` has already silently changed `a[0]`, because they share storage.

**The starter is exactly this shallow version.** It compiles. Run it first: the first case fails (`a[0]` changed), then the scope ends and ASan aborts with a double-free report that shows you both `delete[]` call sites. Read it; you will see this report again in your career.

**The rule of three.** If a class needs a user-written destructor, it almost certainly needs a user-written copy constructor and copy assignment too, because all three exist for the same reason: the object owns a resource. You already have the destructor. Now write the other two.

The copy constructor is the easy one: allocate a fresh array of the same size and copy the elements. `std::copy(first, last, dest)` from `<algorithm>` does the loop.

Copy assignment is harder because `*this` already owns an array that must be released. The subtle case is **self-assignment**: `a = a`, which looks silly written out but happens through aliases in real code (`v[i] = v[j]` with `i == j`, or two references to one object). A naive implementation that frees `data_` first and then copies from `o` is broken when `o` is `*this`: `o.data_` is the pointer you just freed, so the copy reads dead memory and ASan reports `heap-use-after-free` (or, if you already overwrote `data_` with the new allocation, you copy uninitialized garbage onto itself). Either way the contents are gone. Either check `this == &o` up front, or use the copy-and-swap idiom, which is correct without a check:

```cpp
IntBuffer& operator=(const IntBuffer& o) {
    IntBuffer tmp(o);               // 1. deep copy via the copy constructor
    std::swap(data_, tmp.data_);    // 2. take the copy's array
    std::swap(n_, tmp.n_);
    return *this;                   // 3. tmp dies here and frees our old array
}
```

Assignment returns `IntBuffer&`, a reference to `*this`, so that `a = b = c` works: it parses as `a = (b = c)`, and the inner assignment must yield something assignable.

**Two `operator[]`.** Indexing through a `const IntBuffer&` must still be allowed, but must not allow writes. So there are two overloads: the non-`const` one returns `int&` (you can assign through it), the `const` one returns `const int&`. The compiler picks based on whether the object is `const`. Without the `const` overload, `cr[1]` on a `const IntBuffer& cr` is a compile error.

`explicit` on the constructor prevents `IntBuffer b = 5;` from silently meaning "a buffer of five zeros". For a single-argument constructor, `explicit` should be your default.

## Coming from Python

- In Python, `b = a` makes an alias, `copy.copy` is shallow, `copy.deepcopy` is deep. In C++, `=` defaults to a shallow memberwise copy, which for a pointer member is Python's aliasing behavior plus a double free when both objects die.
- `__copy__` and `__deepcopy__` are opt-in hooks that `copy` calls. The C++ copy constructor and copy assignment are called implicitly on every `=`, every pass-by-value, every return-by-value.
- Python's reference counting keeps the shared list alive until the last alias is gone. Here nobody is counting; ownership must be explicit.
- In production C++ you would hold `std::vector<int>` and write none of these three functions: the "rule of zero" (chapter 6). This problem exists so you understand what `vector` is doing on your behalf.

## Your task

Replace the shallow copy constructor and copy assignment with deep copies. Everything else is already written and correct.

```cpp
class IntBuffer {
public:
    explicit IntBuffer(std::size_t n, int fill = 0);
    IntBuffer(const IntBuffer& o);
    IntBuffer& operator=(const IntBuffer& o);
    ~IntBuffer();
    std::size_t size() const;
    int& operator[](std::size_t i);
    const int& operator[](std::size_t i) const;
private:
    int* data_;
    std::size_t n_;
};
```

| sequence | result |
|---|---|
| `IntBuffer a(3, 7); IntBuffer b = a; b[0] = 99;` | `a[0]` is still `7`; no sanitizer report at scope end |
| `IntBuffer a(3, 1); IntBuffer b(5, 2); a = b;` | `a.size()` is `5`, `a[4]` is `2`, and `a`, `b` are independent |
| `IntBuffer a(4, 9); IntBuffer& r = a; a = r;` | `a` still holds four 9s |
| `a = b = c` | all three equal and independent |
| `const IntBuffer& cr = a; cr[1]` | compiles and reads |
| `IntBuffer keep(1); { IntBuffer tmp(2, 5); keep = tmp; } keep[1]` | `5`, after `tmp` is gone |
