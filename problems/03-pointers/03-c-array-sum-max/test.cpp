#include "check.h"
#include "tracked.h"
#include "solution.cpp"

#include <climits>

void run_tests() {
    int a[3] = {1, 2, 3};
    CHECK_EQ("sum_array(a, 3) with a = {1, 2, 3}", sum_array(a, 3), 6LL);
    CHECK_EQ("sum_array(nullptr, 0)", sum_array(nullptr, 0), 0LL);
    int big[2] = {INT_MAX, INT_MAX};
    CHECK_EQ("sum_array(big, 2) with big = {INT_MAX, INT_MAX}", sum_array(big, 2), 4294967294LL);
    int neg[3] = {-1, -2, -3};
    CHECK_EQ("sum_array(neg, 3) with neg = {-1, -2, -3}", sum_array(neg, 3), -6LL);

    int b[3] = {4, 9, 2};
    CHECK_TRUE("max_element_ptr(b, 3) == &b[1] with b = {4, 9, 2}", max_element_ptr(b, 3) == &b[1]);
    int tie[2] = {5, 5};
    CHECK_TRUE("max_element_ptr(tie, 2) == &tie[0] with tie = {5, 5} (first max)", max_element_ptr(tie, 2) == &tie[0]);
    int last[3] = {1, 2, 3};
    CHECK_TRUE("max_element_ptr(last, 3) == &last[2] with last = {1, 2, 3}", max_element_ptr(last, 3) == &last[2]);
    int allneg[3] = {-3, -1, -2};
    CHECK_TRUE("max_element_ptr(allneg, 3) == &allneg[1] with allneg = {-3, -1, -2}", max_element_ptr(allneg, 3) == &allneg[1]);
    CHECK_TRUE("max_element_ptr(nullptr, 0) == nullptr", max_element_ptr(nullptr, 0) == nullptr);
}
