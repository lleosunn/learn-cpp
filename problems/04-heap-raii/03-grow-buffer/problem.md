# Grow a Heap Buffer Without Dangling

A fixed-size block eventually fills up. Growing it means a new block, a copy, and freeing the old one. Getting the order wrong is the classic heap bug, and the starter has it wrong on purpose.

## The lesson

There is exactly one correct order:

```text
1. allocate the new block     p = new Tracked[new_n];
2. copy the survivors         p[i].value = old[i].value   for i < min(old_n, new_n)
3. release the old block      delete[] old;
```

Swap steps 2 and 3 and step 2 reads memory that has already been handed back: a **use-after-free**. The bytes are usually still sitting there, so without a sanitizer this bug can hide for months and then corrupt something unrelated. With AddressSanitizer it aborts on the spot.

**Run the starter before you edit it.** The report you get starts like this (addresses trimmed):

```text
ERROR: AddressSanitizer: heap-use-after-free on address 0x603000000fb0 ...
READ of size 4 at 0x603000000fb0 thread T0
    #0 in grow(Tracked*, unsigned long, unsigned long) solution.cpp:14
    #1 in run_tests() test.cpp:23

0x603000000fb0 is located 16 bytes inside of 28-byte region
freed by thread T0 here:
    #0 in operator delete[]
    #1 in grow(Tracked*, unsigned long, unsigned long) solution.cpp:12
    #2 in run_tests() test.cpp:23

previously allocated by thread T0 here:
    #0 in operator new[]
    #1 in make_block(...) test.cpp:8
```

How to read it:

- The first line names the bug. The second says whether it was a READ or a WRITE and how big.
- The **first stack trace** is where the bad access happened. Find the first frame in `solution.cpp`: line 14 is the crime scene.
- **`freed by`** is where that memory was deleted: line 12. It ran before line 14, so you freed too early.
- **`previously allocated by`** is where the block came from, useful when you are not sure which pointer you are even looking at.
- "16 bytes inside of 28-byte region" is the array cookie from problem 2 showing up: a 16-byte header (element size and element count, 8 bytes each on this arm64 Mac), then three 4-byte `Tracked`.

Two relatives of this bug, both of which ASan names just as precisely:

- Deleting the same block twice: deleting `old` again after `grow` already did, or the caller deleting its old pointer after calling `grow`. ASan calls this `attempting double-free`; for a `new[]` block it usually shows up as a heap-use-after-free instead, because the second `delete[]` reads the element count from the freed cookie before it ever reaches the free.
- Not deleting `old` at all. Nothing crashes; the block just stays alive. The tests catch this because `Tracked::alive` reads 8 instead of 5.

Why this matters: after `grow` returns, the caller's `old` pointer is **dangling by design**. Its bits look exactly like a valid address; nothing marks it dead. The only safe pattern is to overwrite it immediately: `p = grow(p, n, 2 * n);`. Problem 5 wraps this into a class so the dangling pointer can never escape.

## Coming from Python

- `list.append` and `list.extend` do exactly this under the hood (CPython calls `realloc`, which may move the block). You never see it because the list object owns the pointer and updates it.
- Python has no dangling references; an object you can reach is alive by definition. In C++ reachability and liveness are separate, and you maintain the link.
- Python's `min(a, b)` is `std::min(a, b)` from `<algorithm>`. Both arguments must have the same type, so keep sizes as `std::size_t`.

## Your task

`Tracked` is already defined for you:

```cpp
struct Tracked {
    int value;
    static int alive, constructed, destroyed, copies, moves;
    static void reset();
    Tracked(int v = 0);
};
```

The tests use its counters because Apple's clang has no LeakSanitizer.

Return a fresh `new[]` block of `new_n` `Tracked`, with the first `min(old_n, new_n)` values copied from `old` and the rest left at `0`; then release `old`. `old` may be `nullptr` when `old_n == 0`.

```cpp
Tracked* grow(Tracked* old, std::size_t old_n, std::size_t new_n);
```

| call | result |
|---|---|
| `grow({1,2,3}, 3, 5)` | `{1,2,3,0,0}`, `Tracked::alive == 5` |
| `grow({1,2,3}, 3, 2)` | `{1,2}`, `Tracked::alive == 2` |
| `grow(nullptr, 0, 4)` | `{0,0,0,0}`, `Tracked::alive == 4` |

The starter compiles and crashes under AddressSanitizer on the first test. That is the exercise: read the report, find the line, fix the order.
