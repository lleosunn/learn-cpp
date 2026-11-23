# Closures: Adder, Counter, Appender

Python closures are so natural you stop noticing them. C++ has them too, but you must say, per variable, whether the closure gets a copy or a reference, and the compiler holds you to it.

## The lesson

A lambda is an anonymous function object written inline:

```cpp
auto square = [](int x) { return x * x; };
square(4);  // 16
```

The shape is `[captures](parameters) { body }`. The return type is deduced from the `return` statements (spell it out with `-> int` after the parameter list if you need to).

**Captures** are the interesting part. The body can only see outside variables you list in the brackets, and you choose how:

| capture | meaning | Python analogue |
|---|---|---|
| `[n]` | copy `n` into the closure, now | the `lambda x, n=n:` default-argument trick |
| `[&v]` | hold a reference to `v` | an ordinary closure over an outer variable |
| `[=]` / `[&]` | everything the body uses, by copy / by reference | — |
| `[count = 0]` | a new variable owned by the closure (init-capture) | `nonlocal` state |

Two rules that trip up Python programmers:

1. **By-value captures are read-only inside the body.** `[count]() { return ++count; }` does not compile: "cannot assign to a variable captured by copy in a non-mutable lambda". Add `mutable` after the parameter list to let the closure modify its own copies. The copy lives inside the closure object, so it persists from call to call, which is exactly what a counter needs.

2. **By-reference captures keep nothing alive.** If a function captures one of its own locals with `[&]` and returns the lambda, the local dies on return and the closure holds a dangling reference. Under AddressSanitizer this shows up as `stack-use-after-return`. Capture by reference only things that outlive the closure, such as a vector the caller owns and handed you by reference.

```cpp
std::function<int()> broken() {
    int local = 42;
    return [&local] { return local; };  // dangling the moment broken() returns
}
```

**Returning a closure.** Every lambda has its own unnamable type, so a function can't name it as a return type, and callers couldn't store it in a struct field or a vector if it did. `std::function<int(int)>` is a wrapper that can hold *any* callable taking an `int` and returning an `int`: a lambda, a plain function, a function pointer. Returning a lambda from a function declared as `std::function<int(int)>` converts automatically. Problem 2 digs into `std::function` properly.

## Coming from Python

- Python closures capture the *variable*, not the value, which is where the loop-of-lambdas bug comes from. C++ makes you choose per capture; `[n]` is the `n=n` fix built into the language.
- `nonlocal count; count += 1` becomes an init-capture plus `mutable`.
- A Python closure keeps its enclosing variables alive. A C++ closure only keeps what it copied; references inside are just references.
- Lambdas can have full statement bodies with loops and locals, not just one expression.

## Your task

Write three factory functions.

```cpp
std::function<int(int)> make_adder(int n);     // returns f where f(x) == x + n
std::function<int()> make_counter();           // successive calls return 1, 2, 3, ...
std::function<void()> make_appender(std::vector<int>& sink, int value);  // each call pushes value into sink
```

`make_appender` must push into the caller's vector, not a private copy. Separate counters and adders must not share state.

| call | result |
|---|---|
| `make_adder(5)(3)` | `8` |
| `auto c = make_counter(); c(); c(); c();` | `1`, `2`, `3` |
| `auto f = make_appender(sink, 7); f(); f();` | `sink == {7, 7}` |
