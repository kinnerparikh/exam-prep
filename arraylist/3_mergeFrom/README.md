# A3: Merge two sorted ArrayLists

[All problems and setup](../../README.md)

Implement `void NumList::mergeFrom(const NumList &other)`. Copy all of
`other`'s values into this list so that it contains both original sequences
in sorted order.

**Edit:** [mergeFrom.cpp](mergeFrom.cpp).
The working [NumList class](../common/NumList.h) is provided. You do not need
to finish A1 or A2 first.

## Requirements

- **Precondition:** both inputs are sorted in non-decreasing order.
  You do not need to sort or validate unsorted inputs.
- Preserve every occurrence, including duplicates. The resulting size is
  the sum of the two original sizes.
- For distinct objects, `other` must remain unchanged.
- Support self-merge: `list.mergeFrom(list)` duplicates every value in
  sorted order. In this case there is only one object, so it changes.
- Take O(n + m) time, where n and m are the original sizes.
- Allocate **at most one** new array. Free any old array you replace.
  No STL containers or extra temporary arrays.
- Finish with `capacity() >= size()`. Unlike A1's insertion rule, this
  problem does not require the exact `2 * cap + 2` growth sequence.
- Empty inputs are valid. Both lists must remain usable afterward.

It is acceptable to allocate a result array with n + m slots. Additional
space may therefore be O(n + m); an in-place optimization is optional.

## Examples

```cpp
NumList a{1, 4, 9};
NumList b{2, 3, 10};
a.mergeFrom(b);
// a: [1, 2, 3, 4, 9, 10]; b: [2, 3, 10]

NumList c{1, 2};
c.mergeFrom(c);
// c: [1, 1, 2, 2]
```

## Edge cases and hint

Handle empty inputs, one input entirely before the other, interleaved
values, duplicates, negative values, and self-merge with or without spare
capacity.

**Hint:** keep one index in each input. Be careful not to discard source
storage before you have finished reading it, particularly during self-merge.
As an optional challenge, use no allocation when capacity already suffices;
think about filling from the back.

## Check your work

Run from the `exam-prep` folder:

```sh
make al3
make al3 ARGS=self_merge
```

The 22 tests check merged contents, source preservation, self-merge,
allocations and leaks, random inputs, and merging 100,000 + 100,000 elements.
