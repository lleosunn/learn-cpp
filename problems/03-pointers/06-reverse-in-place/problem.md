# Reverse in Place

A practice round: no new syntax. This one drills two pointers moving through the same array and writing through both.

## The lesson

`max_element_ptr` walked one pointer forward. Reversing takes two: one at the front, one at the back, swapping what they point at and stepping toward each other until they meet.

```text
       lo                  hi
       v                   v
     +---+---+---+---+---+- - - -
     | 1 | 2 | 3 | 4 | 5 | ???
     +---+---+---+---+---+- - - -
      arr                 arr+5
```

Where should the back pointer start? `arr + n - 1` is the last element, but only if there is one. With `n == 0` it is an address *before* the array, and even forming that pointer is undefined behavior. The index version has the same hole and fails louder: `std::size_t hi = n - 1;` with `n == 0` wraps around to 18446744073709551615, because `std::size_t` is unsigned.

C++ gives you a safe anchor instead: the **one-past-the-end** pointer, `arr + n`. You may always form it and compare against it. You may never dereference it. Start `hi` there and step it back *before* you use it:

```cpp
int* lo = arr;
int* hi = arr + n;      // legal even when n == 0
while (lo < hi) {
    --hi;               // now hi points at a real element
    // swap *lo and *hi (the three-line swap from problem 2)
    ++lo;
}
```

`lo < hi` compares addresses. That is legal for two pointers into the same array (one-past-the-end included) and means "lo is to the left of hi". When the two meet or cross, every pair has been swapped.

One consequence of "an array is a pointer plus a count": the caller can hand you any slice. `reverse_array(a + 1, 3)` reverses `a[1]`, `a[2]`, `a[3]` and nothing else. Your function cannot tell a slice from a whole array, and it does not need to.

## Coming from Python

- `a.reverse()` is this function. `a[::-1]` is not: it builds a new list. Here nothing is allocated; you rearrange the caller's memory.
- `a[1:4]` copies three elements. `a + 1` with a count of 3 is a *view*: no copy, and writes land in the caller's array.
- There is no negative indexing. `arr[-1]` is not the last element; it is the int in front of the array, which is not yours.

## Your task

Reverse the first `n` ints of `arr` in place.

```cpp
void reverse_array(int* arr, std::size_t n);
```

| call | array afterward |
|---|---|
| `reverse_array(a, 5)` with `a = {1, 2, 3, 4, 5}` | `{5, 4, 3, 2, 1}` |
| `reverse_array(b, 4)` with `b = {1, 2, 3, 4}` | `{4, 3, 2, 1}` |
| `reverse_array(c, 3)` with `c = {1, 2, 3, 4, 5}` | `{3, 2, 1, 4, 5}` (only the first 3) |
| `reverse_array(d + 1, 3)` with `d = {1, 2, 3, 4, 5}` | `{1, 4, 3, 2, 5}` (a slice from the middle) |
| `reverse_array(nullptr, 0)` | nothing happens, no crash |
