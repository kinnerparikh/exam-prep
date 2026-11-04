# R6: Recursive selection sort (medium)

[Recursion problems](../README.md) | [Setup](../../README.md)

Implement `void selectionSort(int values[], int n)` in
[selectionSort.cpp](selectionSort.cpp).

## Requirements

- Sort the first `n` integers in nondecreasing order in the existing array.
- For each nontrivial range, scan it to find a minimum, swap that value
  into the first position, then recursively sort the remaining suffix.
- A loop is allowed for the minimum scan, not for the outer sorting step.
  Use the scan even for an already sorted range.
- No extra arrays, STL containers, or library sorting functions.
- `0 <= n <= 1000`; `nullptr` is allowed for `n == 0`. Never touch elements
  outside the requested range.

## Examples and edge cases

`[3, -2, 3, 0, -2, 1]` becomes `[-2, -2, 0, 1, 3, 3]`.
Handle empty, single-element, already sorted, reversed, and all-equal arrays.

## Analyze before coding

1. Write a recurrence that includes the minimum scan at each level.
2. Expand it into a sum and give tight best- and worst-case time bounds.
3. What is the extra space including recursion? Does "in the existing
   array" mean that the entire algorithm uses constant extra space?
4. Why does one recursive call per level not imply the same time as R1?

## Check your work

Run `make rec6`. Tests check ordering, duplicates, range boundaries, no heap
array allocations, and agreement with an independent library sort.
