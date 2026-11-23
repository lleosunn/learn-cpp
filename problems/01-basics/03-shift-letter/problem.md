# Caesar-Shift a Letter

In Python a character is a length-1 string and you need `ord` and `chr` to get at its number. In C++ a `char` **is** the number; the letter is just how it prints.

## The lesson

`char` is an integer type, one byte wide. The literal `'a'` is not a string: it is the `char` with value 97, its ASCII code. `"a"` with double quotes is something else entirely (a two-byte array: `a` plus a terminating zero), so keep the two kinds of quotes straight.

Because `char` is an integer, you do arithmetic on it directly:

```cpp
char c = 'c';
int idx = c - 'a';                      // 2: position in the alphabet
int next = c + 1;                       // 100: an int, not a char
bool lower = c >= 'a' && c <= 'z';      // plain integer comparisons
```

Two things to notice.

**Arithmetic promotes to `int`.** Any `+ - * / %` with `char` operands is computed in `int`. `'a' + 1` has type `int` and value 98; print it and you see `98`, not `b`. To get a letter back you have to put the value into a `char`:

```cpp
int k = 1;
char d = 'a' + k;                      // implicit narrowing int -> char: compiles, d == 'b'
char e = static_cast<char>('a' + k);   // the same thing, said out loud
char f{'a' + k};                       // brace-init refuses to narrow a runtime int: compile error
```

Narrowing from `int` to `char` keeps the low byte and drops the rest. For 0..127 that is exactly what you want; for 300 it is garbage, and nothing stops you. `static_cast<char>` does not make it safer, it makes it visible: "I know this is an `int` and I mean to shrink it." The same applies to `return`: a function declared to return `char` will silently narrow an `int` expression on the way out.

**The ASCII letters are contiguous.** `'a'..'z'` are 97..122, `'A'..'Z'` are 65..90, `'0'..'9'` are 48..57. So `c - 'a'` maps a lowercase letter to 0..25 and `'a' + n` maps back. That is the entire Caesar cipher: shift the index, wrap with `% 26`, convert back.

Why the offset matters: `(c + k) % 26` is wrong. It takes the remainder of the raw ASCII code, so `'a'` shifted by 1 becomes `98 % 26 == 20`, which is not a letter at all. Subtract `'a'` first, do the math in 0..25, add `'a'` last. (If `k` could be enormous you would reduce it with `k % 26` first so the sum cannot overflow an `int`; the next problem is about that.)

## Coming from Python

- No `ord`/`chr`. `c - 'a'` *is* `ord(c) - ord('a')`; `static_cast<char>('a' + n)` *is* `chr(97 + n)`.
- `'a'` is a `char`, an integer. `"a"` is a string literal. Python does not distinguish; C++ does, and mixing them up is a compile error at best and a silent bug at worst.
- `'a' <= c && c <= 'z'` is how you spell `'a' <= c <= 'z'`; chained comparisons do not exist in C++. `<cctype>` offers `std::islower(c)` and friends if you prefer a named test.
- `%` on a non-negative `int` behaves like Python's. `k` is guaranteed `>= 0` here, so the negative-remainder issue from problem 1 does not arise.

## Your task

Shift a lowercase letter `c` forward by `k` positions in the alphabet, wrapping from `z` back to `a`. `k` is `>= 0` and may exceed 26. Any character that is not a lowercase letter is returned unchanged.

```cpp
char shift_letter(char c, int k);
```

| call | result |
|---|---|
| `shift_letter('a', 1)` | `'b'` |
| `shift_letter('z', 1)` | `'a'` |
| `shift_letter('y', 3)` | `'b'` |
| `shift_letter('m', 26)` | `'m'` |
| `shift_letter('a', 1000)` | `'m'` |
| `shift_letter('A', 1)` | `'A'` |
| `shift_letter('!', 5)` | `'!'` |

The starter returns `c` unchanged, so the "unchanged" cases already pass and the letter cases fail. Make the letters work without breaking the others.
