# R3: Recursive binary search (easy)

[Recursion problems](../README.md) | [Setup](../../README.md)

Implement `int binarySearch(const int values[], int n, int target)` in
[binarySearch.cpp](binarySearch.cpp).

## Requirements

- The first `n` elements are sorted in nondecreasing order.
- Return an index in the original array holding `target`, or `-1` if absent.
  Any matching index is acceptable when there are duplicates.
- Inspect the middle element and recursively search only the relevant half.
- Use index bounds or a pointer plus an offset. Do not copy subarrays,
  allocate memory, or use loops or library search functions.
- `n >= 0`; `nullptr` is allowed only for `n == 0`. Keep the input unchanged.
  You may add private helpers in the `.cpp` file.

## Examples and edge cases

Searching `[-5, 0, 2, 8]` for `2` gives `2`; searching for `3` gives `-1`.
Handle empty input, one element, even and odd lengths, both endpoints, and
duplicates. Make sure a recursive result is still an original-array index.

## Analyze before coding

1. Write the worst-case recurrence in terms of the number of elements `n`.
2. Give tight worst-case time and stack bounds, and best-case time.
3. Why does halving the problem differ from R1's one-element reduction?
4. What changes if each call copies its half before recursing?

## Check your work

Run `make rec3`. Tests check indices, misses, duplicates, prefix bounds, and
a large input. Correct results alone do not prove that you used recursion.
