# A4: Build a circular ArrayList

[All problems and setup](../../README.md)

Implement an array-backed deque: a list that can insert and remove at
either end without shifting every element. Logical contents can wrap from
the end of the array back to index 0.

**Edit:** [CircularArrayList.cpp](CircularArrayList.cpp).
**Interface:** [CircularArrayList.h](CircularArrayList.h).
The constructor, `size()`, `capacity()`, and `frontIndex()` are provided.
Implement the remaining TODOs, including the destructor.

## Representation

Keep the four members `data`, `numItems`, `cap`, and `front`, and do not
change the public interface. Private helpers are allowed. Use a raw heap
array, not `std::vector`, `std::deque`, or another STL container.

A new list has `data == nullptr`, `numItems == 0`, `cap == 0`, and `front == 0`.
For nonempty storage, logical index i maps to `(front + i) % cap`.

```text
physical index:   0    1    2    3    4    5
array contents: [30 | 40 |  ? |  ? | 10 | 20]
front = 4, size = 4, capacity = 6
logical list: [10, 20, 30, 40]
```

## Exact operation rules

The tests check `frontIndex()` as well as contents, so follow these rules.

| Operation | Required behavior |
|-----------|-------------------|
| `expand()` | Grow to `2 * cap + 2`; copy in logical order into indices starting at 0; free the old array; set `front = 0`. |
| `pushAtFront(value)` | Expand first if full, then move `front` one slot left with wrap-around, store the value, and increase size. |
| `pushAtBack(value)` | Expand first if full, write at `(front + numItems) % cap`, and increase size. |
| `popFromFront()` | Move `front` one slot right with wrap-around and decrease size. |
| `popFromBack()` | Decrease size without moving `front`. |
| `elementAt(index)`, `replaceAt(value, index)` | Access the logical index, not the physical array index. |
| `first()`, `last()`, `isEmpty()` | Report the logical front/back value or whether size is zero. |
| `toString()` | Format logical contents as `"[10, 20, 30, 40]"`; empty is `"[]"`. |
| Destructor | Free the heap array. |

Capacity never shrinks. Follow the pop rules even when removing the final
element; do not reset `front` just because the list becomes empty.
Copy construction and assignment are intentionally disabled.

## Errors and complexity

- `elementAt` and `replaceAt` outside `0 <= index < size()` throw
  `std::range_error`.
- `first`, `last`, and either pop on an empty list throw `std::runtime_error`.
  Reject invalid operations before modifying the list.
- Access, replacement, and pops are O(1). Pushes are amortized O(1), with
  O(n) expansion. `toString()` is O(n).

Avoid modulo by zero when capacity is zero. Also remember that C++ computes
`(-1) % 6` as -1, not 5.

## Check your work

Run from the `exam-prep` folder:

```sh
make al4
```

The 29 tests check exact front/capacity changes, wrap-around, expansion while
wrapped, reuse after becoming empty, errors, memory ownership, randomized
operations against a model, and a 200,000-operation front-insertion/removal
performance case.
