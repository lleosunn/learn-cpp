char shift_letter(char c, int k) {
    if (c < 'a' || c > 'z') return c;      // not a lowercase letter: leave it alone
    int idx = c - 'a';                      // 0..25, computed in int
    int shifted = (idx + k) % 26;           // wrap around the alphabet
    return static_cast<char>('a' + shifted);  // int -> char, narrowed on purpose
}
