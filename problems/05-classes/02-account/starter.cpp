#include <string>
#include <utility>

class Account {
public:
    // TODO: this constructor does not compile. Run it, read the error, and move
    // the initialization of owner_ into the member initializer list.
    Account(std::string owner, long long cents = 0) {
        owner_ = owner;
        cents_ = cents;
        // TODO: count this account
    }

    ~Account() {
        // TODO: uncount this account
    }

    const std::string& owner() const { return owner_; }
    long long balance_cents() const { return cents_; }

    void deposit(long long cents) {
        // TODO: ignore non-positive amounts
    }

    bool withdraw(long long cents) {
        // TODO: only if 0 < cents <= balance
        return false;
    }

    static int live() {
        return 0;   // TODO
    }

private:
    const std::string owner_;
    long long cents_;
    static int live_;   // TODO: this is a declaration; it still needs a definition
};
