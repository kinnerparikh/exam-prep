/*
 * mergeSorted.cpp
 *
 * PROBLEM L4 -- void SList::mergeSorted(SList &other)
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

#include <stdexcept>

void SList::mergeSorted(SList &other) {
    // TODO: delete this line and implement mergeSorted
    throw std::logic_error("TODO: implement SList::mergeSorted");
}
