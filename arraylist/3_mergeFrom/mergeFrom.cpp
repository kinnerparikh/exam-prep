/*
 * mergeFrom.cpp
 *
 * PROBLEM A3 -- void NumList::mergeFrom(const NumList &other)
 *
 * PRECONDITION: `*this` and `other` are each sorted in ascending
 * (non-decreasing) order.
 *
 * Merge a copy of every element of `other` into this list so that, when
 * you're done, this list holds all of its original elements AND all of
 * other's elements, still sorted ascending. `other` must not change.
 *
 *   NumList a{1, 4, 9};
 *   NumList b{2, 3, 10};
 *   a.mergeFrom(b);         // a is [1, 2, 3, 4, 9, 10], b is still [2, 3, 10]
 *
 * Requirements
 *   - O(n + m) time, where n = size() and m = other.size(). Inserting each
 *     element of `other` one at a time (with shifting) is O(n * m) and will
 *     time out on the performance test.
 *   - Allocate AT MOST ONE new array, and delete [] any array you replace
 *     (no leaks).
 *   - Afterwards capacity() >= size().
 *   - Self-merge must work:  list.mergeFrom(list)  turns [1, 2] into
 *     [1, 1, 2, 2]. (Careful: `other` and `*this` are the same object!)
 *
 * Hints
 *   - The classic "two-finger" merge: one index into each source array,
 *     repeatedly copy the smaller front element into the destination.
 *   - Simplest approach: allocate a new array of size n + m, merge into it,
 *     THEN free the old array. Think about why "then" matters for self-merge.
 *   - Challenge: if the current capacity is already big enough, you can
 *     merge in place with no allocation by filling the array from the BACK.
 *
 * Run the tests:  make al3
 */

#include "NumList.h"

#include <stdexcept>

void NumList::mergeFrom(const NumList &other) {
    // TODO: delete this line and implement mergeFrom
    throw std::logic_error("TODO: implement NumList::mergeFrom");
}
