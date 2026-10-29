/*
 * removeAll.cpp
 *
 * PROBLEM A2 -- int NumList::removeAll(int value)
 *
 * Remove EVERY occurrence of `value` from the list and return how many
 * elements were removed. The remaining elements must keep their original
 * relative order.
 *
 *   NumList list{4, 1, 4, 4, 2, 4};
 *   list.removeAll(4);      // returns 4, list is now [1, 2]
 *
 * Requirements
 *   - O(n) time: ONE pass over the array. Calling a "remove at index"
 *     routine for every match is O(n^2) and will time out on the
 *     performance test.
 *   - O(1) extra space: do NOT allocate a new/temporary array.
 *   - Capacity does not change.
 *
 * Hint: keep two indices -- one that reads every element, and one that
 * marks where the next "kept" element should be written.
 *
 * Think about it (exam-style): what is the worst-case big-O of the naive
 * approach, and what input triggers it?
 *
 * Run the tests:  make al2
 */

#include "NumList.h"

#include <stdexcept>

int NumList::removeAll(int value) {
    // TODO: delete this line and implement removeAll
    throw std::logic_error("TODO: implement NumList::removeAll");
}
