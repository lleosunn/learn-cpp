double average(int a, int b, int c) {
    // 3.0 is a double, so the int sum is promoted and the division happens in double.
    // (a + b + c) / 3 would be integer division: 5 / 3 == 1, then 1 -> 1.0.
    return (a + b + c) / 3.0;
}
