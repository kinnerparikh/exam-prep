# L3: Delete every matching node

[All problems and setup](../../README.md)

Implement `int SList::removeAll(int value)`: unlink and delete every node
whose `data` equals `value`, then return the number removed.

**Edit:** [removeAll.cpp](removeAll.cpp).
The working [SList class](../common/SList.h) is provided; L1 and L2 do not
need to be completed first.

## Requirements

- Preserve the order and identity of the nodes you keep. Do not overwrite
  their data or replace them with newly allocated nodes.
- Delete every removed node exactly once. Do not allocate replacement nodes.
- Update `front` when removing from the beginning and reduce `numItems`
  by the number removed.
- Preserve the singly linked list invariants: the last `next` is `nullptr`,
  size equals the actual number of nodes, and an empty list has a null front.
- Use one pass, O(n) time. Do not use STL containers.
- Empty lists and lists with no matches return 0.

An iterative solution can use O(1) auxiliary space. A temporary stack dummy
node is allowed; it must not become part of the resulting list.

## Example

```cpp
SList list{4, 4, 1, 4, 2, 4};
int removed = list.removeAll(4);
// removed == 4; list is [1, 2], using the original 1 and 2 nodes
```

## Edge cases and hint

Handle a matching first node, several consecutive matches at the front,
consecutive matches in the middle or at the end, every node matching, no
matches, singleton lists, and repeated calls.

**Hint:** you need a way to update the link that leads into a removed node.
That link may be `front`, not another node's `next`. Never read a node after
you have deleted it.

## Check your work

Run from the `exam-prep` folder:

```sh
make ll3
```

The 19 tests check return counts, contents, size, original surviving node
identities, deletion/leak behavior, and random inputs.

**Optional challenge:** write a recursive helper that returns the new head
of a processed sublist and reports how many nodes it removed. You may add a
private declaration in `SList.h`; do not change the public interface.
Remember to include the recursion stack when analyzing space usage.
