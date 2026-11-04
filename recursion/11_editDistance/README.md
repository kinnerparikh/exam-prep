# R11: Memoized edit distance (hard)

[Recursion problems](../README.md) | [Setup](../../README.md)

Implement `int editDistance(const std::string &a, const std::string &b)` in
[editDistance.cpp](editDistance.cpp).

## Requirements

- Return the fewest single-character insertions, deletions, and replacements
  needed to turn `a` into `b`. Each edit costs one. Matching characters cost
  zero; exchanging adjacent characters is not a single allowed edit.
- Use top-down recursion on a pair of indices into the unchanged strings.
  If the current characters match, advance both indices. Otherwise explore
  the three allowed edits and take the minimum.
- Memoize results by **both** indices so each distinct state is computed
  at most once. Use raw dynamically allocated arrays, not STL containers.
- If either string is empty, return the other length immediately without
  allocating. Otherwise allocate and initialize a full `(m + 1) * (n + 1)`
  memo table, where `m = a.size()` and `n = b.size()`. A flattened array is
  fine. Use a sentinel that cannot be confused with a valid distance.
- Free the table before returning. Do not retain memo entries between public
  calls. Loops are allowed for initialization/cleanup, not a bottom-up DP
  solution. No substring copies or string-by-value recursive parameters.
- Inputs are case-sensitive ASCII strings, each at most 200 characters.

## Examples and edge cases

`"kitten" -> "sitting"` needs 3 edits; `"flaw" -> "lawn"` needs 2.
`"" -> "abc"` needs 3; identical strings need 0; `"ab" -> "ba"` needs 2,
not 1. Spaces are ordinary characters.

## Analyze before coding

1. Define the recursive state and its base cases. Write the recurrence for
   the **answer** at a state with matching and nonmatching characters.
2. For running time, count distinct states and work per state rather than
   charging for every path through the uncached recursion tree.
3. Give tight worst-case time in terms of **both** lengths, including table
   initialization. What if one input is empty? What if both have length `N`?
4. Give table space and maximum recursion depth separately.
5. If the strings are identical, does exploring fewer states eliminate the
   cost of initializing the required full table?

## Check your work

Run `make rec11`. Tests cover each operation, empty inputs, repeated
characters, a bottom-up oracle, independent calls, and a large mismatch
case that is impractical without memoization. Array leaks are checked.
