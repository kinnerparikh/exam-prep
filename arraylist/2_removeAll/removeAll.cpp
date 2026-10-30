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

/*
 * two-index walkthrough: removeAll(4) from [4, 1, 4, 4, 2, 4]
 *
 * fastIndex reads each element; slowIndex is the next write position.
 * Only [0, slowIndex) contains the kept elements so far.
 *
 * fastIndex   Read   Action                 slowIndex after   Kept prefix
 *     0        4     Skip (matches value)          0           []
 *     1        1     data[0] = data[1]             1           [1]
 *     2        4     Skip (matches value)          1           [1]
 *     3        4     Skip (matches value)          1           [1]
 *     4        2     data[1] = data[4]             2           [1, 2]
 *     5        4     Skip (matches value)          2           [1, 2]
 *
 * Final backing array (capacity unchanged):
 *       index:   0   1 | 2   3   4   5
 *       data:  [ 1,  2 | 4,  4,  2,  4 ]
 *               kept  | ignored old contents
 *                     ^ slowIndex = new numItems = 2
 *
 * Removed = old numItems - slowIndex = 6 - 2 = 4.
 * Writes never overtake reads, so kept elements stay in order.
 * One pass: O(n) time. Two indices, no new array: O(1) extra space.
 */
int NumList::removeAll(int value) {
    
    int slowIndex = 0;
    
    for (int fastIndex = 0; fastIndex < numItems; fastIndex++) {
        if (data[fastIndex] != value) {
            data[slowIndex++] = data[fastIndex];
        }
    }

    int totRemoved = numItems - slowIndex;
    numItems = slowIndex;
    return totRemoved;
}
