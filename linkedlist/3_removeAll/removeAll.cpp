/*
 * removeAll.cpp
 *
 * PROBLEM L3 -- int SList::removeAll(int value)
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

#include "SList.h"

#include <stdexcept>

int SList::removeAll(int value) {
    // TODO: delete this line and implement removeAll
    throw std::logic_error("TODO: implement SList::removeAll");
}
