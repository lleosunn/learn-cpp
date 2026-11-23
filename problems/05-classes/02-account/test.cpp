#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    {
        Account a("alice");
        CHECK_EQ("new account starts at 0", a.balance_cents(), 0LL);
        a.deposit(500);
        CHECK_EQ("deposit 500", a.balance_cents(), 500LL);
        CHECK_TRUE("withdraw 200 succeeds", a.withdraw(200));
        CHECK_EQ("balance after withdraw", a.balance_cents(), 300LL);
        CHECK_TRUE("withdraw 1000 from 300 fails", !a.withdraw(1000));
        CHECK_EQ("failed withdraw leaves balance", a.balance_cents(), 300LL);
        a.deposit(-5);
        CHECK_EQ("deposit -5 is ignored", a.balance_cents(), 300LL);
        CHECK_TRUE("withdraw -1 fails", !a.withdraw(-1));
        CHECK_EQ("balance unchanged after bad withdraw", a.balance_cents(), 300LL);
        CHECK_TRUE("withdraw exact balance succeeds", a.withdraw(300));
        CHECK_EQ("balance is 0 after withdrawing everything", a.balance_cents(), 0LL);
    }
    {
        Account a("x", 250);
        CHECK_EQ("owner()", a.owner(), std::string("x"));
        CHECK_EQ("opening balance", a.balance_cents(), 250LL);
    }
    {
        CHECK_EQ("live() == 0 before any account", Account::live(), 0);
        Account a("x");
        Account b("y");
        CHECK_EQ("live() == 2 with two accounts", Account::live(), 2);
        {
            Account c("z");
            CHECK_EQ("live() == 3 in nested scope", Account::live(), 3);
        }
        CHECK_EQ("live() == 2 after nested scope", Account::live(), 2);
    }
    CHECK_EQ("live() == 0 after all destroyed", Account::live(), 0);
}
