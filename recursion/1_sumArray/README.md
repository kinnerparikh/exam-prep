# R1: Sum an array (easy)

[Recursion problems](../README.md) | [Setup](../../README.md)

Implement `long long sumArray(const int values[], int n)` in
[sumArray.cpp](sumArray.cpp).

## Requirements

- Return the sum of the first `n` elements without modifying the array.
- An empty array sums to zero; `values` may be `nullptr` when `n == 0`.
- Use one recursive call on a problem with one fewer element.
- Do not use loops, allocate arrays, or copy the input.
- `0 <= n <= 1000`. The sum fits in `long long`, but need not fit in `int`.
  Invalid inputs are outside the contract.

## Examples and edge cases

`[4, -7, 0, 9, -2]` gives `4`; `[]` gives `0`; `[-8]` gives `-8`.
Watch for negative values, cancellation, and a sum larger than `INT_MAX`.

## Analyze before coding

1. What is the base case? Write a recurrence for time as a function of `n`.
2. How much work happens outside the recursive call?
3. Give a tight worst-case Big-O bound and the maximum call-stack space.
4. Would splitting the array into two halves change the total time?

## Check your work

Run `make rec1` from the repository root. Tests check results, prefix
boundaries, unchanged input, and wide sums. You must check the recursive
structure yourself; passing output tests does not establish a time bound.
