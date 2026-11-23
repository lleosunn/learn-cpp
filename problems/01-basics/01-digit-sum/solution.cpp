int digit_sum(int n) {
    if (n < 0) n = -n;
    int sum = 0;
    while (n > 0) {
        sum += n % 10;  // last digit
        n /= 10;        // drop it (integer division)
    }
    return sum;
}
