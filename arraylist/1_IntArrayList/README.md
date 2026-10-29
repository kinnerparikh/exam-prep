# A1: Build an IntArrayList

[All problems and setup](../../README.md)

Implement a resizable list of integers backed by a heap array. Practice
indexing, shifting, capacity management, and the Big Three: destructor, copy
constructor, and copy assignment.

**Edit:** [IntArrayList.cpp](IntArrayList.cpp).
**Interface:** [IntArrayList.h](IntArrayList.h).
The default constructor, `size()`, `capacity()`, and `toString()` are provided.
Implement the remaining TODOs, including the empty destructor.

## Requirements

- Keep the public interface and the `data`, `numItems`, and `cap` members.
  Private helpers are allowed; declare them in the header if needed.
- Use `new int[...]` and `delete[]`, not STL containers.
- The elements occupy `data[0..numItems-1]`, with `0 <= numItems <= cap`.
  A newly constructed empty list has `data == nullptr`, size 0, and capacity 0.
- Before inserting into a full array, `expand()` must grow capacity to
  `2 * cap + 2`, copy the elements in order, and free the old array.
  The growth sequence is 0, 2, 6, 14, 30, ...
- The array constructor copies the first `length` integers and sets both
  size and capacity to `length`. A length of 0 behaves like the default
  constructor. Assume a valid input array and nonnegative length.
- Copies must own independent arrays. Assignment must release the old
  storage, handle `a = a`, and return `*this`. The destructor frees storage.

| Operation | Required behavior |
|-----------|-------------------|
| `isEmpty()` | Return whether size is zero. |
| `first()`, `last()`, `elementAt(index)` | Return the requested value without modifying the list. |
| `replaceAt(value, index)` | Replace one value without changing size. |
| `indexOf(value)` | Return the first matching index, or -1. |
| `pushAtFront(value)`, `pushAtBack(value)` | Insert at the chosen end. |
| `insertAt(value, index)` | Insert before the current element at `index`; `index == size()` appends. |
| `popFromFront()`, `popFromBack()`, `removeAt(index)` | Remove one element, preserving the order of the others. |
| `clear()` | Set size to zero but keep the allocated capacity. |
| `shrinkToFit()` | Make capacity equal size; when empty, free storage and set `data` to `nullptr`. |

Indices are zero-based. Notice the **value, index** parameter order.
Normal removals do not shrink capacity. Output uses `"[1, 2, 3]"` and `"[]"`.

## Errors

| Invalid call | Exception |
|--------------|-----------|
| `elementAt`, `replaceAt`, or `removeAt` outside `0 <= index < size()` | `std::range_error` |
| `insertAt` outside `0 <= index <= size()` | `std::range_error` |
| `first`, `last`, or either pop on an empty list | `std::runtime_error` |

An invalid operation must leave the list unchanged. Capacity is not the
same as size: an unused array slot is not a valid list index.

## Complexity targets

Access and replacement are O(1). Append is amortized O(1), but O(n) when
expansion is needed. Front insertion/removal and middle insertion/removal
are O(n). Removing from the back is O(1).

## Example

```cpp
IntArrayList list;
list.pushAtBack(10);
list.pushAtFront(5);
list.insertAt(7, 1);   // [5, 7, 10], size 3, capacity 6
list.removeAt(0);      // [7, 10], capacity still 6
list.shrinkToFit();    // capacity is now 2
```

## Check your work

Run from the `exam-prep` folder:

```sh
make al1
make al1 ARGS=insertAt
```

The 67 tests cover empty/single-element lists, index boundaries, shifting,
growth, copying and self-assignment, exceptions, memory ownership, and
random operation sequences. Leave tests and provided functions unchanged.
