#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    int x = 1, y = 2;
    int* p = &x;
    int* q = &y;
    swap_pointers(&p, &q);
    CHECK_TRUE("p == &y after swap_pointers(&p, &q) with p = &x, q = &y", p == &y);
    CHECK_TRUE("q == &x after swap_pointers(&p, &q)", q == &x);
    CHECK_EQ("x is still 1 (the ints do not move)", x, 1);
    CHECK_EQ("y is still 2", y, 2);

    int* s = &x;
    swap_pointers(&s, &s);
    CHECK_TRUE("s == &x after swap_pointers(&s, &s)", s == &x);
    CHECK_EQ("x is still 1 after swap_pointers(&s, &s)", x, 1);

    int arr[3] = {10, 20, 30};
    int* first = arr;
    int* last = arr + 2;
    swap_pointers(&first, &last);
    CHECK_TRUE("first == arr + 2 after swap_pointers(&first, &last)", first == arr + 2);
    CHECK_TRUE("last == arr after swap_pointers(&first, &last)", last == arr);
    CHECK_EQ("arr is still {10, 20, 30}", std::vector<int>(arr, arr + 3), (std::vector<int>{10, 20, 30}));

    int* r = &x;
    int* n = nullptr;
    swap_pointers(&r, &n);
    CHECK_TRUE("r == nullptr after swap_pointers(&r, &n) with n = nullptr", r == nullptr);
    CHECK_TRUE("n == &x after swap_pointers(&r, &n)", n == &x);
}
