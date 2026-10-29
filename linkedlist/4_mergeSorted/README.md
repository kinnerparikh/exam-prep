# L4: Merge sorted lists by moving nodes

[All problems and setup](../../README.md)

Implement `void SList::mergeSorted(SList &other)`. Move `other`'s existing
nodes into this list, keeping the combined list sorted and leaving `other`
empty.

**Edit:** [mergeSorted.cpp](mergeSorted.cpp).
The working [SList class](../common/SList.h) is provided. This problem can
be completed independently of L1-L3.

## Requirements

- **Precondition:** both lists are sorted in non-decreasing order.
  You do not need to sort or validate unsorted inputs.
- Re-link the original nodes. Do not copy data into replacement nodes,
  change node values, or delete any input nodes.
- The resulting list contains every original node exactly once.
- Make the merge **stable**: preserve the order within each input, and when
  values tie, put this list's nodes before `other`'s equal-valued nodes.
- Set this list's size to the sum of the original sizes. For distinct
  lists, set `other.front = nullptr` and `other.numItems = 0`.
- Both objects must remain valid: destroying or reusing the emptied `other`
  must not affect the merged list.
- Self-merge (`a.mergeSorted(a)`) is a no-op, not a duplication or deletion.
- Use O(n + m) time and O(1) extra space with an **iterative** implementation.
  No STL containers or extra result nodes. A temporary stack dummy node is
  allowed; it must not become part of the final list.

All list tails must end in `nullptr`, with no cycles or shared ownership.
Unlike A3, this operation **moves nodes and empties the source** rather than
copying its values.

## Examples

```cpp
SList a{1, 4, 9};
SList b{2, 3, 10};
a.mergeSorted(b);
// a: [1, 2, 3, 4, 9, 10]; b: []
```

For a tie, label the nodes by their original list:

```text
a: [2a] -> [2b]      other: [2c] -> [3d]
result: [2a] -> [2b] -> [2c] -> [3d]
```

## Edge cases and hint

Handle both empty, either empty, singleton lists, all of one list before
the other, interleaved values, duplicate values, and self-merge.

**Hint:** track the tail of the result and the first unprocessed node of
each input. When one input runs out, consider what can be done with the
remaining chain.

## Check your work

Run from the `exam-prep` folder:

```sh
make ll4
make ll4 ARGS=ties
```

The 19 tests check contents, node identities and stable ordering, sizes,
source reuse/destruction, self-merge, memory, random inputs, and merging
300,000 + 300,000 nodes.
