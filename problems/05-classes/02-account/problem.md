# Bank Account with Static Counter

A constructor is where a class's invariants are established. Here there are three: the owner's name is fixed for the life of the account, the balance never goes negative, and the class knows at all times how many accounts exist.

## The lesson

```cpp
class Account {
public:
    Account(std::string owner, long long cents = 0)
        : owner_(std::move(owner)), cents_(cents) { ++live_; }
    ~Account() { --live_; }

    const std::string& owner() const { return owner_; }
    static int live() { return live_; }

private:
    const std::string owner_;
    long long cents_;
    static int live_;
};

int Account::live_ = 0;   // the one definition of the static member
```

- **`class` is private by default.** Convention: the public interface first, then a `private:` section with the data. The trailing underscore on `cents_` is just a naming convention for members; it is not syntax.
- **A `const` data member** can never be assigned after construction. So where does its value come from? Only from the member initializer list. Writing `owner_ = owner;` in the constructor body is an assignment, and the compiler rejects it: `error: no viable overloaded '='`, followed by a note that explains why: `'this' argument has type 'const std::string' ... but method is not marked const`. In other words, `std::string::operator=` modifies the string, and `owner_` is const. **The starter does exactly that, so it does not compile.** Run it first, read the error, then move the initialization into the list.
- **Initialization order.** Members are initialized in the order they are *declared*, regardless of the order you write in the initializer list. If the two orders differ, clang warns (`-Wreorder-ctor`, part of `-Wreorder`): `field 'cents_' will be initialized after field 'owner_'`. Keep them the same.
- **`std::move(owner)`** moves the string out of the by-value parameter into the member instead of copying it a second time. Chapter 7 explains moves; for now, treat "take by value, `std::move` into the member" as the standard idiom.
- **The destructor `~Account()`** runs automatically, immediately, the moment an object goes out of scope. Chapter 4 used it to free memory; here it decrements a counter. Because every creation runs a constructor and every destruction runs the destructor, the count cannot drift. (Copies are the one exception: a copied object skips your constructor. The tests here never copy. Problem 4 is about that.)
- **`static` members belong to the class, not to any object.** There is one `live_` total, like a Python class attribute. Inside the class, `static int live_;` is only a *declaration*. It needs exactly one definition at namespace scope: `int Account::live_ = 0;`. Forget it and the code compiles but fails to link: `Undefined symbols ... Account::live_`. Learn to recognize that message; it means "declared, never defined". In C++17 you can instead write `inline static int live_ = 0;` inside the class and skip the separate definition.
- **A `static` member function** such as `live()` has no `this`. You call it as `Account::live()`, no object required.

One more convention: money is in integer cents (`long long`), never `double` dollars. Floating point cannot represent 0.10 exactly.

## Coming from Python

- A Python class attribute (`Account.live = 0`) versus an instance attribute (`self.cents`) is the same split as `static` versus non-static, except the static must be defined separately.
- Python has no `const`; here the compiler enforces that `owner_` never changes. "Private" in Python is a leading-underscore convention; `private:` here is enforced.
- `__del__` may run late or never. `~Account()` is guaranteed and runs at a predictable point.
- `@staticmethod` corresponds to a `static` member function.

## Your task

Complete `Account`:

- The constructor records the owner and opening balance and increments the live-account counter; the destructor decrements it.
- `owner()` returns the owner's name; `balance_cents()` the balance.
- `deposit(cents)` adds to the balance; non-positive amounts are ignored.
- `withdraw(cents)` subtracts and returns `true` if `cents` is positive and does not exceed the balance; otherwise it returns `false` and the balance is unchanged.
- `Account::live()` returns how many `Account` objects currently exist.

```cpp
class Account {
public:
    Account(std::string owner, long long cents = 0);
    ~Account();
    const std::string& owner() const;
    long long balance_cents() const;
    void deposit(long long cents);
    bool withdraw(long long cents);
    static int live();
private:
    const std::string owner_;
    long long cents_;
    static int live_;
};
```

| sequence | result |
|---|---|
| `Account a("x"); a.deposit(500); a.withdraw(200);` | returns `true`, balance `300` |
| then `a.withdraw(1000)` | returns `false`, balance still `300` |
| then `a.deposit(-5)` | ignored, balance still `300` |
| `a.owner()` | `"x"` |
| `{ Account a("x"); Account b("y"); Account::live(); }` | `2` inside the braces, `0` after |
