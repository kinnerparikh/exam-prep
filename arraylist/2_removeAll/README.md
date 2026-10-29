# A2: Remove every matching value

[All problems and setup](../../README.md)

Implement `int NumList::removeAll(int value)`: remove every occurrence of
`value` from an array-backed list and return the number removed.

**Edit:** [removeAll.cpp](removeAll.cpp).
The working [NumList class](../common/NumList.h) is provided; this problem
does not depend on your A1 implementation.

## Requirements

- Preserve the relative order of all remaining elements.
- Update `numItems` to the new size and return the number removed.
- Use O(n) time: one pass over the original elements.
- Use O(1) extra space. Do not allocate a new array or use STL containers.
- Keep the same array and capacity, including when every element is removed.
  Unused slots beyond the new size do not need to be cleared.
- Leave the list valid for later operations. An empty list or a list with
  no matches returns 0 without changing its contents.

The provided representation is `data[0..numItems-1]`, with `cap` allocated
slots. Work directly with these members; do not modify the provided
implementation or tests.

## Example

```cpp
NumList list{4, 1, 4, 4, 2, 4};
int removed = list.removeAll(4);
// removed == 4; list is [1, 2]; its capacity has not changed
```

## Edge cases and hint

Handle empty lists, a single matching or nonmatching element, matches at
either end, consecutive matches, all matches, no matches, and negative values.

**Hint:** distinguish the position you are reading from the position where
the next kept value belongs. Repeatedly shifting the suffix for each removal
can take O(n^2).

## Check your work

Run from the `exam-prep` folder:

```sh
make al2
```

The 19 tests check order, count, size, unchanged capacity, allocation behavior,
random inputs, and a 200,000-element performance case.
