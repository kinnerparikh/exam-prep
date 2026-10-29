/*
 * mergeFrom.cpp
 *
 * PROBLEM A3 -- void NumList::mergeFrom(const NumList &other)    -- SOLUTION
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

/* ---------------------------------------------------------------------------
 * SOLUTION NOTES
 * ---------------------------------------------------------------------------
 * Two versions are below. Flip the switch to try the challenge version;
 * both pass every test.
 *
 *   0 = simple:   merge FRONT-to-back into a brand-new n+m array.
 *   1 = in-place: merge BACK-to-front, reusing the array when cap >= n+m.
 *
 * THE TWO-FINGER MERGE (the core of both versions, and of merge sort):
 *
 *   this : [1, 4, 9]        i -> next unused element of this
 *   other: [2, 3, 10]       j -> next unused element of other
 *
 *   compare 1 vs 2  -> take 1     out: [1]
 *   compare 4 vs 2  -> take 2     out: [1, 2]
 *   compare 4 vs 3  -> take 3     out: [1, 2, 3]
 *   compare 4 vs 10 -> take 4     out: [1, 2, 3, 4]
 *   compare 9 vs 10 -> take 9     out: [1, 2, 3, 4, 9]
 *   `this` is used up -> copy the rest of other: [1, 2, 3, 4, 9, 10]
 *
 * Every step copies exactly one element and moves exactly one finger,
 * so the merge does n + m steps: O(n + m).
 * ------------------------------------------------------------------------- */
#define USE_IN_PLACE_MERGE 0

#include "NumList.h"

#if !USE_IN_PLACE_MERGE

/*
 * VERSION 0 -- simple: merge into a brand-new array of exactly n + m slots.
 *
 * Time  O(n + m)    Extra space O(n + m) (the new array)
 */
void NumList::mergeFrom(const NumList &other) {
    /* Read both sizes NOW. On a self-merge, `other` IS `*this`, so
       other.numItems would change as soon as we update numItems below. */
    int n = numItems;
    int m = other.numItems;

    /* Nothing to merge in: leave the list (and its array) alone. */
    if (m == 0) {
        return;
    }

    int *merged = new int[n + m];   /* the ONE allowed allocation */

    int i = 0;   /* next unused element of this->data */
    int j = 0;   /* next unused element of other.data */
    int k = 0;   /* next free slot in merged          */

    /* While both lists still have elements, copy the smaller one.
       On a tie (<=) we take from `this` first. Any choice gives the same
       sorted ints, but taking the left side first is what makes merge
       sort "stable" -- a good habit. */
    while (i < n && j < m) {
        if (data[i] <= other.data[j]) {
            merged[k] = data[i];
            i++;
        } else {
            merged[k] = other.data[j];
            j++;
        }
        k++;
    }

    /* Exactly one of these loops actually runs: whichever list still
       has leftovers. Those leftovers are already sorted and are all
       >= everything copied so far, so we just copy them over. */
    while (i < n) {
        merged[k] = data[i];
        i++;
        k++;
    }
    while (j < m) {
        merged[k] = other.data[j];
        j++;
        k++;
    }

    /* Free the old array ONLY NOW, after the merge is finished.
       On a self-merge, other.data and data are the SAME array. If we had
       deleted it before merging, the loops above would have read freed
       memory (undefined behavior). */
    delete [] data;

    data     = merged;
    numItems = n + m;
    cap      = n + m;
}

#else /* USE_IN_PLACE_MERGE */

/*
 * VERSION 1 -- challenge: fill from the BACK.
 *
 * If cap >= n + m, the merged list fits in our existing array. But merging
 * front-to-back in place would overwrite data[0], data[1], ... before
 * we'd read them. So merge from the back instead: place the LARGEST
 * remaining element into the LAST free slot.
 *
 *   data (cap 6): [1, 4, 9, ?, ?, ?]  n = 3     other: [2, 3, 10]  m = 3
 *                        i=2                                 j=2   k=5
 *
 *   9 vs 10 -> data[5]=10  j=1 k=4     [1, 4, 9, ?, ?, 10]
 *   9 vs 3  -> data[4]=9   i=1 k=3     [1, 4, 9, ?, 9, 10]
 *   4 vs 3  -> data[3]=4   i=0 k=2     [1, 4, 9, 4, 9, 10]
 *   1 vs 3  -> data[2]=3   j=0 k=1     [1, 4, 3, 4, 9, 10]
 *   1 vs 2  -> data[1]=2   j=-1 k=0    [1, 2, 3, 4, 9, 10]
 *   other is used up -> done: data[0..i] are already in the right place!
 *
 * Why this is safe: k always equals i + j + 1, so the write slot k is
 * always > i and > j. We never overwrite an element we still need to
 * read -- even on a self-merge, where other.data is the same array.
 *
 * If cap is too small we still need one new array; then the same
 * back-to-front loop writes into it instead, and we copy any leftovers
 * from `this` too, since they aren't already in that array.
 *
 * Time  O(n + m)    Extra space O(1) when cap >= n + m
 */
void NumList::mergeFrom(const NumList &other) {
    int n = numItems;          /* read sizes first (self-merge!) */
    int m = other.numItems;

    if (m == 0) {
        return;
    }

    int *dest     = data;      /* where we write: our own array... */
    bool newArray = (n + m > cap);
    if (newArray) {
        dest = new int[n + m]; /* ...or a fresh one if it won't fit */
    }

    int i = n - 1;             /* last unused element of this->data */
    int j = m - 1;             /* last unused element of other.data */
    int k = n + m - 1;         /* next slot to fill, moving left     */

    /* Run until other is used up. Take from `this` only if it is
       strictly bigger. On a tie, take from `other`, so the elements
       from `this` stay further left -- stability when filling backward. */
    while (j >= 0) {
        if (i >= 0 && data[i] > other.data[j]) {
            dest[k] = data[i];
            i--;
        } else {
            dest[k] = other.data[j];
            j--;
        }
        k--;
    }

    if (newArray) {
        /* Leftovers from `this` (data[0..i]) still need copying into
           the NEW array. Then free the old array -- after merging, for
           the same self-merge reason as in version 0. */
        while (i >= 0) {
            dest[k] = data[i];
            i--;
            k--;
        }
        delete [] data;
        data = dest;
        cap  = n + m;
    }
    /* In place: leftovers data[0..i] are already exactly where they
       belong (k == i at this point), so there is nothing to copy. */

    numItems = n + m;
}

#endif /* USE_IN_PLACE_MERGE */
