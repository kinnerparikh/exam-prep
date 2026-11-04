# R8: Count ways to climb stairs (medium)

[Recursion problems](../README.md) | [Setup](../../README.md)

Implement `long long countStairs(int n)` in
[countStairs.cpp](countStairs.cpp).

## Requirements

- Starting at the bottom of `n` stairs, each move climbs either one or two.
  Return the number of ordered move sequences that reach exactly the top.
- There is one way to climb zero stairs: make no moves.
- For `n >= 2`, recursively explore both first-move choices and add their
  counts. Use the base cases for zero and one stair.
- Deliberately do not memoize, use loops, or use a closed-form formula.
  This exercise is about repeated subproblems; optimization is a paper
  follow-up, not part of the implementation.
- `0 <= n <= 30`. Inputs are kept small because the required recursion
  repeats work.

## Examples and edge cases

For `n == 3`, the sequences are `(1,1,1)`, `(1,2)`, and `(2,1)`, so return
`3`. For `n == 4`, return `5`. Move order matters.

## Analyze before coding

1. Write the recurrence for running time, not just for the returned count.
2. Draw the call tree for `n == 5`. Which subproblems repeat?
3. Give an exponential Big-O bound, then find a tighter one by solving
   `x^2 = x + 1`. Explain why not all branches have the same size.
4. How much stack space is live at once? Do not confuse it with total calls.
5. If you cached each distinct `n`, how would time and extra space change?

## Check your work

Run `make rec8`. Tests check counting and base cases at manageable sizes.
Tests cannot distinguish memoization from the required uncached algorithm;
review your code against the requirements before analyzing it.
