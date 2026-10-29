# L2: Reverse a singly linked list

[All problems and setup](../../README.md)

Implement `void SList::reverse()` by reversing the links between existing
nodes, not by moving their values.

**Edit:** [reverse.cpp](reverse.cpp).
The working [SList class](../common/SList.h) is provided, so this problem
does not depend on L1. It has a `front` pointer and `numItems`, but no back
pointer.

## Requirements

- Reverse the order of the original nodes in place.
- Change `front` to the new first node and make the new last node's `next`
  equal `nullptr`. Keep `numItems` unchanged.
- Do not create or delete nodes, swap `data` values, or build a second list.
- Use O(n) time and O(1) extra space. No arrays, vectors, stacks, or other
  STL containers.
- Use an **iterative** implementation for the required solution.
- Empty and single-node lists remain valid and unchanged.

## Example

```text
before: front -> [1] -> [2] -> [3] -> nullptr
after:  front -> [3] -> [2] -> [1] -> nullptr
```

Reversing twice must restore both the original values and the original node
order. Later insertions should still work.

**Hint:** draw the roles of the previous, current, and next nodes. Before
changing a link, consider how you will reach the still-unprocessed nodes.

## Check your work

Run from the `exam-prep` folder:

```sh
make ll2
```

The 13 tests cover small lists, duplicates, repeated reversal, original node
identities, memory, random inputs, and a 1,000,000-node performance case.
Simply swapping integer values will not pass the identity checks.

**Optional challenge:** after finishing the iterative version, try a recursive
private helper (you may declare it in `SList.h`). Account for O(n) call-stack
space and possible stack overflow on the large test; restore the iterative
version for the required solution.
