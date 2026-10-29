/*
 * reverse.cpp
 *
 * PROBLEM L2 -- void SList::reverse()    -- SOLUTION
 *
 * Reverse the list IN PLACE by re-linking the existing nodes.
 *
 *   before:  front -> [1] -> [2] -> [3] -> nullptr
 *   after:   front -> [3] -> [2] -> [1] -> nullptr
 *
 * Requirements
 *   - O(n) time, O(1) extra space (no arrays/vectors/stacks, no second list).
 *   - Do NOT create or delete any nodes, and do NOT just swap the `data`
 *     values -- the tests check that the SAME nodes appear in reverse order.
 *   - Must be iterative (the performance test uses a list long enough to
 *     overflow the call stack with naive recursion).
 *
 * Hint: walk the list once with THREE pointers: prev, curr, next.
 *       Draw the picture after one step of the loop before writing code.
 *
 * Challenge (after the tests pass): write a recursive version as a private
 * helper, e.g.  Node *reverseHelper(Node *prev, Node *curr);  that returns
 * the new front (add the declaration to SList.h). Every test except the
 * 1,000,000-node performance test should pass with it -- why does that one
 * crash? (Each recursive call uses stack space.)
 *
 * Exam-style questions to think about:
 *   - Why is reversing a DOUBLY-linked list easier? What would you change?
 *   - What's the big-O of reversing an ArrayList in place?
 *
 * Run the tests:  make ll2
 */

#include "SList.h"

/*
 * ---------------------------------------------------------------------------
 * SOLUTION: three pointers -- prev, curr, next
 * ---------------------------------------------------------------------------
 * Walk the list once and flip each node's `next` to point BACKWARD.
 *
 *   prev = the part we've ALREADY reversed (starts empty: nullptr)
 *   curr = the node we're flipping now
 *   next = the rest of the ORIGINAL list -- we must save it before
 *          flipping curr->next, or we'd lose the rest of the list!
 *
 * Trace on [1] -> [2] -> [3] -> null:
 *
 *   start:   prev = null    curr = [1]
 *
 *   step 1:  next = [2]
 *            [1]->next = null              reversed: [1] -> null
 *            prev = [1], curr = [2]
 *
 *   step 2:  next = [3]
 *            [2]->next = [1]               reversed: [2] -> [1] -> null
 *            prev = [2], curr = [3]
 *
 *   step 3:  next = null
 *            [3]->next = [2]               reversed: [3] -> [2] -> [1] -> null
 *            prev = [3], curr = null   -> loop ends
 *
 *   front = prev = [3]
 *
 * Empty list: curr starts as nullptr, the loop never runs, front = prev =
 * nullptr -- still correct with no special case. Same for one node.
 *
 * Time O(n) (each node visited once), extra space O(1) (three pointers).
 * numItems doesn't change. No nodes are created or deleted.
 */
void SList::reverse() {
    Node *prev = nullptr;
    Node *curr = front;

    while (curr != nullptr) {
        Node *next = curr->next;   /* 1. save the rest of the list    */
        curr->next = prev;         /* 2. flip this node's arrow        */
        prev = curr;               /* 3. curr joins the reversed part  */
        curr = next;               /* 4. move on to the rest           */
    }

    front = prev;   /* the old LAST node is the new front */
}

/*
 * ---------------------------------------------------------------------------
 * The recursive version (the "challenge") -- shown here for study only.
 * ---------------------------------------------------------------------------
 * It needs a private helper declared in SList.h:
 *       Node *reverseHelper(Node *prev, Node *curr);
 *
 *   Node *SList::reverseHelper(Node *prev, Node *curr) {
 *       if (curr == nullptr) {          // base case: walked off the end,
 *           return prev;                //   prev is the old last node
 *       }
 *       Node *next = curr->next;
 *       curr->next = prev;              // flip one arrow
 *       return reverseHelper(curr, next);
 *   }
 *
 *   void SList::reverse() { front = reverseHelper(nullptr, front); }
 *
 * It's the same algorithm -- the loop variables just became parameters.
 * Why does it crash on the 1,000,000-node performance test? Each call
 * pushes a stack frame and none of them return until the end, so it needs
 * O(n) stack space. The call stack is only a few MB, and a million frames
 * overflow it (a segfault). The loop needs O(1) space.
 *
 * ---------------------------------------------------------------------------
 * Exam questions from the header
 * ---------------------------------------------------------------------------
 * - Doubly-linked list: every node already knows both neighbors. Just swap
 *   each node's prev and next pointers, then swap front and back. No need
 *   to save "next" separately -- it's still reachable via the (swapped)
 *   prev pointer. Still O(n).
 * - ArrayList in place: swap data[i] with data[n-1-i] for i < n/2. That's
 *   n/2 swaps -> O(n) time, O(1) extra space.
 */
