#include <string>
#include <utility>

class Account {
public:
    // const members can only be set in the initializer list; same order as declared.
    Account(std::string owner, long long cents = 0)
        : owner_(std::move(owner)), cents_(cents) {
        ++live_;
    }
    ~Account() { --live_; }   // runs when the object dies, so the count never drifts

    const std::string& owner() const { return owner_; }
    long long balance_cents() const { return cents_; }

    void deposit(long long cents) {
        if (cents > 0) cents_ += cents;
    }

    bool withdraw(long long cents) {
        if (cents <= 0 || cents > cents_) return false;
        cents_ -= cents;
        return true;
    }

    static int live() { return live_; }

private:
    const std::string owner_;
    long long cents_;
    static int live_;   // declaration only
};

int Account::live_ = 0;   // the single definition, at namespace scope
