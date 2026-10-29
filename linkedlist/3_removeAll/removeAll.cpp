/*
 * removeAll.cpp
 *
 * PROBLEM L3 -- int SList::removeAll(int value)    -- SOLUTION
 *
 * Remove (and delete) EVERY node whose data equals `value`. Return how many
 * nodes were removed. The remaining nodes keep their order.
 *
 *   SList list{4, 4, 1, 4, 2, 4};
 *   list.removeAll(4);      // returns 4, list is now [1, 2]
 *
 * Requirements
 *   - O(n) time: one pass.
 *   - delete every removed node (no leaks); do NOT create new nodes, and do
 *     NOT touch the nodes you keep (other than their next pointers).
 *   - Keep numItems correct.
 *
 * The tricky cases are all about the FRONT: what if the first node matches?
 * The first three? Every node? Get those right and the rest is easy.
 *
 * Hint: there are three classic ways to handle "the front might change":
 *   (a) a loop that removes matches at the front first, then a loop that
 *       looks one node AHEAD (curr->next) for the rest;
 *   (b) a temporary "dummy" node placed before front;
 *   (c) a pointer-to-pointer (Node **) that points at whichever `next`
 *       field (or `front` itself) leads to the current node.
 * Try more than one!
 *
 * Challenge: write it RECURSIVELY with a private helper such as
 *       Node *removeAllHelper(Node *curr, int value, int &count);
 * that returns the new head of the remaining list. (Add the declaration to
 * SList.h.) This is a very common exam question shape.
 *
 * Run the tests:  make ll3
 */

/*
 * ---------------------------------------------------------------------------
 * SOLUTION NOTES
 * ---------------------------------------------------------------------------
 * All four approaches from the hints are implemented below. Change this
 * number to pick one -- every version passes every test.
 *
 *   1 = (a) remove matches at the front, then look AHEAD with curr->next
 *   2 = (b) a temporary "dummy" node before front
 *   3 = (c) pointer-to-pointer (Node **)
 *   4 = the recursive challenge (uses removeAllHelper, declared in SList.h)
 *
 * WHY "LOOK AHEAD"? In a SINGLY-linked list, to remove node X you must
 * change the `next` of the node BEFORE X -- and X has no prev pointer.
 * So we stand on the node before (curr) and inspect curr->next.
 *
 * GOLDEN RULE for every version: never touch a node after you delete it.
 * Save whatever you need from it (its `next`) BEFORE `delete`.
 * ------------------------------------------------------------------------- */
#define REMOVE_ALL_VERSION 1

#include "SList.h"

#if REMOVE_ALL_VERSION == 1

/*
 * VERSION 1 -- front loop + look-ahead loop.
 *
 * Phase 1: while the FRONT node matches, pop it. This handles
 *          [4, 4, 1, ...] and even "every node matches".
 * Phase 2: now front (if any) is definitely NOT a match, so it's a safe
 *          place to stand. Walk with curr and look at curr->next:
 *
 *     curr        curr->next      curr->next->next
 *     [ 1 ] -->   [ 4 ]   -->     [ 2 ]
 *
 *   match: unlink it ->  [ 1 ] --> [ 2 ]   then delete [4].
 *          Do NOT advance curr: the NEW curr->next ([2]) hasn't been
 *          checked yet (think of [1, 4, 4, 2]).
 *   no match: advance curr = curr->next.
 */
int SList::removeAll(int value) {
    int removed = 0;

    /* Phase 1: matches at the front. */
    while (front != nullptr && front->data == value) {
        Node *doomed = front;
        front = front->next;      /* save the link BEFORE deleting */
        delete doomed;
        removed++;
    }

    /* Phase 2: the rest, looking one node ahead. */
    Node *curr = front;           /* nullptr if the list is now empty */
    while (curr != nullptr && curr->next != nullptr) {
        if (curr->next->data == value) {
            Node *doomed = curr->next;
            curr->next = doomed->next;   /* bypass the doomed node */
            delete doomed;
            removed++;
            /* stay on curr -- check its new next on the next iteration */
        } else {
            curr = curr->next;
        }
    }

    numItems -= removed;
    return removed;
}

#elif REMOVE_ALL_VERSION == 2

/*
 * VERSION 2 -- dummy node.
 *
 * The front is only special because it has no node before it. So give it
 * one! Put a fake node in front of the list:
 *
 *     dummy -> [4] -> [4] -> [1] -> [4] -> [2] -> null
 *
 * Now EVERY real node has a predecessor, and one look-ahead loop handles
 * everything -- no special front case. At the end, the real list starts
 * at dummy.next (which may have changed, or become nullptr).
 *
 * The dummy is a LOCAL variable (on the stack), not `new`: it is
 * destroyed automatically when the function returns. (Its constructor and
 * destructor bump the live-node counter up and back down, so the leak
 * checks still balance. If you used `new Node`, you'd have to delete it.)
 */
int SList::removeAll(int value) {
    int removed = 0;
    Node dummy(0, front);         /* value is irrelevant; next = front */

    Node *curr = &dummy;
    while (curr->next != nullptr) {
        if (curr->next->data == value) {
            Node *doomed = curr->next;
            curr->next = doomed->next;
            delete doomed;
            removed++;
        } else {
            curr = curr->next;
        }
    }

    front = dummy.next;           /* the first REAL node (or nullptr) */
    numItems -= removed;
    return removed;
}

#elif REMOVE_ALL_VERSION == 3

/*
 * VERSION 3 -- pointer to pointer (Node **).
 *
 * `link` points at the POINTER that leads to the current node. At first
 * that pointer is `front` itself; later it's some node's `next` field.
 *
 *     link = &front                 link = &([1]->next)
 *        |                                  |
 *        v                                  v
 *     front --> [4] --> [1] --> ...   [1].next --> [4] --> [2]
 *
 * *link is the current node. To remove it:   *link = (*link)->next;
 * That ONE line updates `front` or some node's `next` -- whichever
 * `link` points at -- so the front needs no special case.
 *
 * To keep the current node, advance: link = &(*link)->next;
 * (read as: "the address of the current node's next field").
 */
int SList::removeAll(int value) {
    int removed = 0;
    Node **link = &front;

    while (*link != nullptr) {
        Node *curr = *link;
        if (curr->data == value) {
            *link = curr->next;   /* whoever pointed at curr now skips it */
            delete curr;
            removed++;
            /* don't advance: *link is now the next unchecked node */
        } else {
            link = &curr->next;   /* move to the pointer inside curr */
        }
    }

    numItems -= removed;
    return removed;
}

#elif REMOVE_ALL_VERSION == 4

/*
 * VERSION 4 -- recursive (the "challenge" / common exam shape).
 *
 * removeAllHelper(curr, value, count) returns the front of the list that
 * results from removing every `value` from the sublist starting at curr.
 *
 * Think of it as "trust the recursion":
 *   - Base case: an empty list stays empty -> return nullptr.
 *   - First fix up the REST of the list:  rest = helper(curr->next, ...)
 *   - Then decide about curr itself:
 *       match    -> delete curr, return rest   (curr disappears)
 *       no match -> curr->next = rest, return curr
 *
 * The caller just does  front = removeAllHelper(front, value, count);
 * which also handles "the front changed" automatically.
 *
 * Cost: O(n) time, but O(n) STACK space (one frame per node), so on a
 * very long list it can overflow the stack -- that's why the iterative
 * versions are preferred in practice. (The L3 tests use short lists.)
 */
SList::Node *SList::removeAllHelper(Node *curr, int value, int &count) {
    if (curr == nullptr) {
        return nullptr;
    }

    Node *rest = removeAllHelper(curr->next, value, count);

    if (curr->data == value) {
        delete curr;              /* we already saved curr->next via rest */
        count++;
        return rest;
    }
    curr->next = rest;
    return curr;
}

int SList::removeAll(int value) {
    int removed = 0;              /* passed by reference, so helpers add to it */
    front = removeAllHelper(front, value, removed);
    numItems -= removed;
    return removed;
}

#endif /* REMOVE_ALL_VERSION */
