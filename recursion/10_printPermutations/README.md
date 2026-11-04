# R10: Print every permutation (hard)

[Recursion problems](../README.md) | [Setup](../../README.md)

Implement `void printPermutations(const std::string &text, std::ostream &out)`
in [printPermutations.cpp](printPermutations.cpp).

## Requirements

- Print every permutation exactly once, in any order, one per line with a
  final newline. The empty string has one permutation: print `"\n"`.
- Input contains at most 9 distinct printable ASCII characters. Spaces
  and different letter cases count as different characters; repeated
  characters and embedded newlines are outside this problem's contract.
- Make one mutable working copy of the input. At each position, loop over
  the remaining choices, swap a choice into place, recurse, and undo the
  swap before considering the next choice.
- Pass the same working string by reference through the recursion. No
  substring copies, per-branch string copies, permutation library calls,
  or containers that retain all the output.
- Keep the input unchanged and append to the supplied stream.

## Example

For `"abc"`, print these six lines in any order:

```text
abc
acb
bac
bca
cab
cba
```

## Analyze before coding

1. At a node with `k` positions left, how many recursive children are there?
2. Write a recurrence that includes the choice loop. At a leaf, printing
   still costs time proportional to the **original** string length `n`.
3. Give tight total time including output, stack space, and working-copy
   space. Why is counting just the leaves not the full runtime?
4. What goes wrong if you do not undo a swap? Why does allowing duplicate
   input characters require a different uniqueness strategy?

## Check your work

Run `make rec10`. Tests use an independent permutation oracle and check
all lines, duplicates, literal spaces/case, unchanged input, and independent
calls. They do not require a particular output order.
