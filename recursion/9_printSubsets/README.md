# R9: Print every subset (hard)

[Recursion problems](../README.md) | [Setup](../../README.md)

Implement `void printSubsets(const int values[], int n, std::ostream &out)`
in [printSubsets.cpp](printSubsets.cpp).

## Requirements

- Print every subset exactly once, including the empty subset. Subset
  lines may appear in any order, but values within each subset must keep
  their original input order.
- Write one subset per line as `{1, 2}`: braces, comma followed by one space,
  no trailing comma, and a final newline. The empty subset is `"{}\n"`.
- At each position, recursively explore excluding and including its value.
  Keep the current selection in one reusable raw array and backtrack.
- Loops are allowed to print a completed selection, not to enumerate
  bitmasks or replace the recursive decision tree.
- Do not collect all subsets in memory, copy an array per branch, or use
  STL containers. Release any allocated buffer before returning.
- `0 <= n <= 16`; input values are distinct integers. `nullptr` is allowed
  for `n == 0`. Keep the input unchanged and append to the supplied stream.

## Example

For `[1, 2]`, one acceptable output is:

```text
{}
{2}
{1}
{1, 2}
```

An empty input produces one line, not zero lines. Negative values and zero
use their normal decimal representation.

## Analyze before coding

1. How many leaves and total calls are in the include/exclude tree?
2. Count the total number of integers written across all output lines.
   Give a tight time bound **including output**, not just branching.
3. Give stack and buffer space separately. Exclude storage owned by the
   caller's output stream from auxiliary space.
4. If you only counted subsets instead of printing them, would the output
   lower bound still apply? Distinguish the same tree from a direct formula.

## Check your work

Run `make rec9`. Tests compare complete output against a bitmask-based
oracle, ignoring line order but checking formatting, multiplicity, input
order within each subset, and array-buffer leaks.
