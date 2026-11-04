# R7: Merge sort with one shared buffer (medium)

[Recursion problems](../README.md) | [Setup](../../README.md)

Implement `void mergeSort(int values[], int n)` in
[mergeSort.cpp](mergeSort.cpp).

## Requirements

- Sort the first `n` integers in nondecreasing order.
- Recursively sort two approximately equal halves, then merge them using
  a linear scan. Always perform the merge, even if the input was sorted.
- Allocate exactly one `new int[n]` scratch array in the public function
  when `n >= 2`, share it among recursive calls, and `delete[]` it afterward.
  Allocate nothing for `n < 2`.
- Helpers should use bounds into the original array and the scratch array.
  Loops are allowed for merging and copying back, not instead of recursion.
- Do not use STL containers or library sorting functions.
- `0 <= n <= 100000`; `nullptr` is allowed when `n == 0`. Elements beyond
  the requested range must stay unchanged.

## Examples and edge cases

`[4, -1, 4, 0, -8, 2, 2]` becomes `[-8, -1, 0, 2, 2, 4, 4]`.
Handle uneven halves, duplicate values, and either half running out first.
Each merge must respect its own subrange of the shared buffer.

## Analyze before coding

1. Write the recurrence, including both recursive calls and the merge.
2. How much work occurs at each level, and how many levels are there?
3. Give tight best- and worst-case time bounds for the required algorithm.
4. Separate the stack bound from the scratch-array space. Why does adding
   two recursive calls not necessarily make an algorithm exponential?

## Check your work

Run `make rec7`. Tests cover values, boundaries, odd/even sizes, one shared
allocation with no leaks, and a 100,000-element input.
