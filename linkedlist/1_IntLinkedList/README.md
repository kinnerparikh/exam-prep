# L1: Build a doubly linked list

[All problems and setup](../../README.md)

Implement a list of integers with `front` and `back` pointers and nodes
containing `data`, `prev`, and `next`. Practice pointer updates, ownership,
the Big Three, and empty/single-node edge cases.

**Edit:** [IntLinkedList.cpp](IntLinkedList.cpp).
**Interface:** [IntLinkedList.h](IntLinkedList.h).
The default constructor, `size()`, and forward `toString()` are provided.
Implement the remaining TODOs, including the destructor.

## Invariants and ownership

- Keep the public interface, Node definition, and existing data members.
  Private helpers are allowed; add declarations in the header if needed.
- Do not use STL containers. Allocate nodes with
  `new Node(value, prevPtr, nextPtr)` and release them with `delete`.
- An empty list has `front == nullptr`, `back == nullptr`, and `numItems == 0`.
- In a nonempty list, `front->prev == nullptr` and `back->next == nullptr`.
  Adjacent nodes must link back to each other.
- `numItems` always equals the actual number of nodes. No cycles,
  dangling links, leaks, or shared ownership between copies.

## Required operations

| Operation | Required behavior |
|-----------|-------------------|
| Array constructor | Copy `arr[0..length-1]` in order into new nodes. Assume valid input and nonnegative length. |
| Copy constructor, `operator=` | Make independent deep copies. Assignment deletes old nodes, handles self-assignment, and returns `*this`. |
| Destructor, `clear()` | Delete every node. After `clear()`, restore the empty-list invariants. |
| `isEmpty()` | Return whether the list has no nodes. |
| `first()`, `last()`, `elementAt(index)` | Return the requested value. |
| `replaceAt(value, index)` | Replace the requested value without changing size. |
| `indexOf(value)` | Return the index of the first match, or -1. |
| `pushAtFront(value)`, `pushAtBack(value)` | Insert at the chosen end, updating both directions. |
| `insertAt(value, index)` | Insert before the current element; index equal to size appends. |
| `insertInOrder(value)` | Assuming sorted input, insert before the first value greater than or equal to `value`, or append if none exists. |
| Pops and `removeAt(index)` | Unlink and delete exactly one node; preserve all remaining values in order. |
| `toReverseString()` | Walk from `back` using `prev`; do not reverse the forward string. |

Indices are zero-based; insertion/replacement parameters are **value, index**.
Forward/reverse output uses `"[1, 2, 3]"` / `"[3, 2, 1]"`; empty is `"[]"`.

## Errors

| Invalid call | Exception |
|--------------|-----------|
| `elementAt`, `replaceAt`, or `removeAt` outside `0 <= index < size()` | `std::range_error` |
| `insertAt` outside `0 <= index <= size()` | `std::range_error` |
| `first`, `last`, or either pop on an empty list | `std::runtime_error` |

Reject invalid operations without changing the list.

## Complexity targets and example

Both-end pushes/pops and `first`/`last` should be O(1).
Indexed access, indexed insertion/removal, search, and sorted insertion may
take O(n). Copying and clearing take O(n).

```cpp
IntLinkedList list;
list.pushAtBack(1);
list.pushAtBack(5);
list.insertInOrder(3);  // [1, 3, 5]
list.popFromFront();   // [3, 5]; reverse string is "[5, 3]"
```

For every pointer update, consider the front, back, only-node, and middle
cases. Save any pointer you still need before deleting a node.

## Check your work

Run from the `exam-prep` folder:

```sh
make ll1
make ll1 ARGS=copy
```

The 52 tests check both traversal directions, boundaries, exceptions,
sorted insertion, deep copies, self-assignment, node lifetimes, and random
operation sequences. Do not alter the node counters or tests.
