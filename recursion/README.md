# Recursion practice

[All problems and setup](../README.md)

11 independent C++ exercises: **4 easy, 4 medium, and 3 hard**. Each has a
starter function, a header, tests, and its own problem statement. You do not
need to finish any ArrayList or LinkedList implementation first.

| # | Level | Problem | File to implement | Main idea |
|---|---|---|---|---|
| R1 | Easy | [Sum an array](1_sumArray/README.md) | `1_sumArray/sumArray.cpp` | one-element reduction |
| R2 | Easy | [Count decimal digits](2_countDigits/README.md) | `2_countDigits/countDigits.cpp` | numeric value versus input length |
| R3 | Easy | [Binary search](3_binarySearch/README.md) | `3_binarySearch/binarySearch.cpp` | discard half the search range |
| R4 | Easy | [Palindrome check](4_isPalindrome/README.md) | `4_isPalindrome/isPalindrome.cpp` | two-ended recursion and early exit |
| R5 | Medium | [Exponentiation by squaring](5_fastPower/README.md) | `5_fastPower/fastPower.cpp` | reuse a recursive result |
| R6 | Medium | [Recursive selection sort](6_selectionSort/README.md) | `6_selectionSort/selectionSort.cpp` | nonconstant work at each call |
| R7 | Medium | [Merge sort](7_mergeSort/README.md) | `7_mergeSort/mergeSort.cpp` | split, solve both halves, merge |
| R8 | Medium | [Count stair-climbing sequences](8_countStairs/README.md) | `8_countStairs/countStairs.cpp` | branching with repeated subproblems |
| R9 | Hard | [Print all subsets](9_printSubsets/README.md) | `9_printSubsets/printSubsets.cpp` | include/exclude backtracking |
| R10 | Hard | [Print all permutations](10_printPermutations/README.md) | `10_printPermutations/printPermutations.cpp` | swap, recurse, undo |
| R11 | Hard | [Memoized edit distance](11_editDistance/README.md) | `11_editDistance/editDistance.cpp` | recursion with a two-parameter cache |

Difficulty reflects implementing the required approach and reasoning about
it, not just how quickly the function runs. Work in table order, or compare
R1/R6, R3/R7, R5/R8, and R9/R10 to see why similar-looking recursion can
have different costs.

## Rules

- Replace the `logic_error("TODO ...")` in each starter. Keep public
  signatures unchanged; put any helpers in the same `.cpp` file.
- Use recursion for the main algorithm. Follow each problem's specific
  rules about loops, copying, allocation, and memoization.
- Use raw arrays when a buffer/table is needed, and release them. No
  `std::vector`, `std::list`, maps, or sets in your implementation.
  `std::string` and streams are allowed where the interface calls for them.
  Tests may use STL containers and independent iterative oracles.
- Inputs satisfy the documented preconditions; you do not need to invent
  invalid-input exceptions. Do not edit the tests or test framework.
- Starters intentionally compile but fail tests until implemented.
  Correct outputs do not prove recursion, complexity, or compliance with
  every allocation rule: review your code as well as running tests.

## Complexity worksheet

Before coding each problem, record:

1. The input-size variable(s), base case, and smaller recursive subproblem(s).
2. A recurrence for running time, including work outside recursive calls.
3. A tight worst-case Big-O bound and a justification by expansion,
   recursion-tree levels, counting states, or counting required output.
4. Maximum stack depth and total auxiliary space, including buffers/caches.
5. Any best-case shortcut or change caused by copying, repeated calls,
   memoization, or printing the results.

Use the standard unit-cost model for fixed-width arithmetic, comparisons,
array indexing, and character output. For R2 and R5, analyze growth with
numeric value/exponent despite finite C++ integer limits; this is not
arbitrary-precision bit-complexity analysis. For printing integers in R9,
their fixed-width decimal representation has bounded cost.

Analyze nontrivial inputs asymptotically and handle zero/empty cases
separately. Include output time for R9/R10, but exclude the caller's stored
output from auxiliary space. Count stack frames even if an optimizing
compiler could eliminate tail calls.

The separate complexity answer key is kept in `recursion/ANSWER_KEY.md`
**on `kinner-branch` only**. It is intentionally absent from the practice
branches and from individual problem statements.

## Running

From the repository root in the POSIX environment described in the
[setup instructions](../README.md):

```sh
make rec1                     # build and run R1
make rec7 ARGS=buffer          # filter tests
make recursion-easy           # R1-R4
make recursion-medium         # R5-R8
make recursion-hard           # R9-R11
make recursion                # all eleven, with a summary
make SANITIZE=1 rec11          # memory/undefined-behavior checks
```

Individual targets are `rec1` through `rec11`; `make test` also includes
these alongside the existing data-structure exercises. As with the other
practice targets, `make rec1` keeps going after test failures: read the
summary. Running `./build/rec1` directly returns a nonzero exit status on
failure.
