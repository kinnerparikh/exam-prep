/*
 * reverse.cpp
 *
 * PROBLEM L2 -- void SList::reverse()
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

#include <stdexcept>

void SList::reverse() {
    // TODO: delete this line and implement reverse
    throw std::logic_error("TODO: implement SList::reverse");
}
