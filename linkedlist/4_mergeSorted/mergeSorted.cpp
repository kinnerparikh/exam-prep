/*
 * mergeSorted.cpp
 *
 * PROBLEM L4 -- void SList::mergeSorted(SList &other)    -- SOLUTION
 *
 * PRECONDITION: `*this` and `other` are each sorted ascending.
 *
 * MOVE every node of `other` into this list so that this list ends up
 * sorted ascending and holds all the nodes of both lists. Afterwards `other`
 * must be a valid EMPTY list (front == nullptr, size 0).
 *
 *   SList a{1, 4, 9};
 *   SList b{2, 3, 10};
 *   a.mergeSorted(b);       // a is [1, 2, 3, 4, 9, 10], b is []
 *
 * Requirements
 *   - Do NOT create or delete any nodes -- only re-link `next` pointers.
 *     (Compare with problem A3, where merging arrays had to COPY values.)
 *   - O(n + m) time, O(1) extra space, iterative (the performance test uses
 *     lists long enough to overflow the stack with naive recursion).
 *   - Stable: when values are equal, nodes from `this` come BEFORE nodes
 *     from `other`. (The tests check node identities, so this matters.)
 *   - Keep both numItems values correct.
 *   - If `other` is the same object as `*this`, do nothing.
 *
 * Hint: keep a `tail` pointer to the last node of the merged result. Each
 * step, attach the smaller of the two current nodes to tail and advance.
 * A temporary dummy node -- or a Node ** pointing at "the next pointer to
 * fill in" -- removes the special case for the first node. (A dummy node
 * created with new counts as creating a node, so either delete it before
 * you return or put it on the stack:  Node dummy(0, nullptr);  )
 *
 * Run the tests:  make ll4
 */

#include "SList.h"

/*
 * ---------------------------------------------------------------------------
 * SOLUTION: dummy node + tail pointer
 * ---------------------------------------------------------------------------
 * This is the same two-finger merge as problem A3 -- but instead of
 * COPYING values into a new array, we RE-LINK the existing nodes. No new
 * nodes, no deleted nodes, O(1) extra space.
 *
 *   a    = next unused node of this list
 *   b    = next unused node of other
 *   tail = LAST node of the merged result so far (we append after it)
 *
 * The "dummy" is a fake node before the merged result, so tail always
 * points at a real node and appending never needs an "is the result
 * still empty?" special case. The real merged list starts at dummy.next.
 *
 * Trace: this = [1, 4],  other = [2, 3, 10]
 *
 *   dummy -> ?           a=[1] b=[2]   1 <= 2 -> take a
 *   dummy -> 1           a=[4] b=[2]   4 >  2 -> take b
 *   dummy -> 1 -> 2      a=[4] b=[3]   4 >  3 -> take b
 *   dummy -> 1 -> 2 -> 3 a=[4] b=[10]  4 <= 10 -> take a
 *   dummy -> 1 -> 2 -> 3 -> 4         a=null -> stop
 *   attach the rest of b:  ... -> 4 -> 10
 *   front = dummy.next = [1]
 *
 * STABILITY: on a tie we take from `a` (this list) first -- that's why
 * the test is  a->data <= b->data  (not <).
 *
 * The dummy lives on the STACK (Node dummy(...), no `new`), so it is
 * destroyed automatically when the function returns. Its constructor and
 * destructor bump the live-node counter up then back down, so the leak
 * checks balance. (If you `new` it, you'd have to `delete` it.)
 *
 * Alternative without a dummy: a pointer-to-pointer  Node **link = &front;
 * then each step does  *link = chosen; link = &chosen->next;
 * -- same idea as L3 version 3.
 *
 * Time O(n + m). Extra space O(1).
 */
void SList::mergeSorted(SList &other) {
    /* Merging a list with itself: the spec says do nothing. (Otherwise
       a and b would walk the same nodes and we'd tangle the list.) */
    if (&other == this) {
        return;
    }

    Node dummy(0, nullptr);
    Node *tail = &dummy;
    Node *a = front;
    Node *b = other.front;

    while (a != nullptr && b != nullptr) {
        if (a->data <= b->data) {      /* <= keeps it stable */
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;             /* the node we just attached */
    }

    /* One list ran out. The other's leftovers are already sorted and are
       already linked together, so ONE pointer assignment attaches them
       all -- no loop needed (unlike the array version in A3). */
    if (a != nullptr) {
        tail->next = a;
    } else {
        tail->next = b;
    }

    front = dummy.next;

    /* Bookkeeping: we now own all the nodes, and `other` must be a valid
       EMPTY list. If we left other.front pointing at its old first node,
       BOTH lists would think they own it -> double delete later. */
    numItems      += other.numItems;
    other.front    = nullptr;
    other.numItems = 0;
}
