# Event Bus: Callbacks with std::function

In Python an event bus is a `dict[str, list[Callable]]`. Same shape here, except a vector needs a single element type and every lambda has a different one. `std::function` is the fix.

## The lesson

**`std::function<Sig>` is a type-erased callable.** `std::function<void(int)>` can hold any object you can call with an `int`: a lambda with captures, a plain function, a function pointer, a class with `operator()`. The concrete type is hidden ("erased") behind one uniform type, so a mix of them fits in a single `std::vector<std::function<void(int)>>`.

```cpp
void print_it(int x) { std::cout << x << "\n"; }

std::vector<std::function<void(int)>> handlers;
handlers.push_back(print_it);                              // plain function
handlers.push_back([](int x) { std::cout << x * 2; });     // lambda
int total = 0;
handlers.push_back([&total](int x) { total += x; });       // capturing lambda
for (const auto& h : handlers) h(5);
```

The cost is an indirect call and possibly a heap allocation for a big closure. Use `std::function` when you need to *store* callables of unknown type; when you only *call* one, as `filter` does in the next problem, a template parameter is cheaper.

Three facts you need here:

- **Copying a `std::function` copies the callable inside it**, captures included. Take the handler parameter by value and `std::move` it into storage so there is one copy, not two.
- **Calling an empty `std::function` throws `std::bad_function_call`.** A default-constructed one is empty; `if (h)` tests for that.
- **A `const` member function cannot use `map::operator[]`.** `handlers_[event]` *inserts* a default entry when the key is missing. That modifies the map, so it is not available on a `const` object. `emit` is `const` because emitting must never change the subscription table, and because a stray `emit("typo", 1)` must not grow it with an empty entry. Use `find`:

```cpp
auto it = handlers_.find(event);
if (it == handlers_.end()) return 0;   // nothing subscribed
// it->first is the key, it->second is the vector of handlers
```

`const` after a member function's parameter list is a promise that the function will not modify the object. The compiler enforces it by only letting you call other `const` members inside, and it is what makes a `const EventBus&` parameter trustworthy. Forget the `const` on `emit` and the test that emits through a `const EventBus&` will not compile.

## Coming from Python

- `dict[str, list[Callable[[int], None]]]` becomes `std::map<std::string, std::vector<std::function<void(int)>>>`. The annotation is now enforced.
- `d.setdefault(k, []).append(h)` is exactly `handlers_[event].push_back(h)`: `operator[]` default-constructs the missing vector.
- `d.get(k, [])` is `find` plus an `end()` check. `std::map` has no `get` with a default.
- A bare function name is a valid handler in both languages. In C++ it decays to a function pointer, which `std::function` accepts.

## Your task

Implement `EventBus`.

```cpp
class EventBus {
public:
    using Handler = std::function<void(int)>;

    void subscribe(const std::string& event, Handler h);            // append h to event's list
    std::size_t emit(const std::string& event, int payload) const;  // call every handler in subscription order; return how many
    void clear(const std::string& event);                           // drop all handlers for event

private:
    std::map<std::string, std::vector<Handler>> handlers_;
};
```

| scenario | result |
|---|---|
| subscribe `h1`, `h2` to `"tick"`, then `emit("tick", 4)` | `h1(4)` then `h2(4)`; returns `2` |
| `emit("nope", 1)` with nothing subscribed | returns `0`, table unchanged |
| `clear("tick")` then `emit("tick", 1)` | returns `0` |
