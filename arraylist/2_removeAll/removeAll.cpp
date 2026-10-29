/*
 * removeAll.cpp
 *
 * PROBLEM A2 -- int NumList::removeAll(int value)    -- SOLUTION
 *
 * Remove EVERY occurrence of `value` from the list and return how many
 * elements were removed. The remaining elements must keep their original
 * relative order.
 *
 *   NumList list{4, 1, 4, 4, 2, 4};
 *   list.removeAll(4);      // returns 4, list is now [1, 2]
 *
 * Requirements
 *   - O(n) time: ONE pass over the array.
 *   - O(1) extra space: do NOT allocate a new/temporary array.
 *   - Capacity does not change.
 *
 * ---------------------------------------------------------------------------
 * THE IDEA: "read" and "write" indices (a.k.a. compaction)
 * ---------------------------------------------------------------------------
 * `read`  visits every element exactly once.
 * `write` is where the next element we KEEP should go.
 *
 * Because write <= read at all times, writing to data[write] never
 * overwrites an element we haven't read yet. Kept elements slide left over
 * the removed ones, in their original order.
 *
 * Trace for removeAll(4) on [4, 1, 4, 4, 2, 4]:
 *
 *   read=0  data[0]=4  match -> skip                 write=0
 *   read=1  data[1]=1  keep  -> data[0]=1            write=1   [1,1,4,4,2,4]
 *   read=2  data[2]=4  match -> skip                 write=1
 *   read=3  data[3]=4  match -> skip                 write=1
 *   read=4  data[4]=2  keep  -> data[1]=2            write=2   [1,2,4,4,2,4]
 *   read=5  data[5]=4  match -> skip                 write=2
 *
 *   Now numItems = write = 2 -> the list is [1, 2].
 *   Whatever is left in data[2..5] is just spare capacity now.
 *
 * ---------------------------------------------------------------------------
 * Exam answer: why not call removeAt(i) on every match?
 * ---------------------------------------------------------------------------
 * Each removeAt shifts everything after it, which is O(n). With k matches
 * that's O(k * n). The worst case is "every element matches" (or most do,
 * near the front): about n + (n-1) + ... + 1 = O(n^2) shifts. The two-index
 * version moves each kept element at most once: O(n) total.
 *
 * Run the tests:  make al2
 */

#include "NumList.h"

int NumList::removeAll(int value) {
    int write = 0;

    for (int read = 0; read < numItems; read++) {
        if (data[read] != value) {
            /* Keep it: copy it down to the next "kept" slot. (When nothing
               has been removed yet, read == write and this copies an
               element onto itself, which is harmless.) */
            data[write] = data[read];
            write++;
        }
        /* else: skip it -- not copying it is what "removes" it */
    }

    int removed = numItems - write;   /* how many we skipped */
    numItems = write;                 /* the kept elements are data[0..write-1] */
    return removed;                   /* cap is untouched, no new array made */
}
