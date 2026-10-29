/*
 * reverse_tests.cpp -- tests for PROBLEM L2 (SList::reverse).
 *
 * Run all:   make ll2
 */

#include "SList.h"
#include "test_framework.h"

#include <algorithm>
#include <vector>

using IntVec  = std::vector<int>;
using AddrVec = std::vector<const void *>;

static int nodes() { return SList::liveNodeCount(); }

template <typename T>
static std::vector<T> reversed(std::vector<T> v) {
    std::reverse(v.begin(), v.end());
    return v;
}

TEST(empty_list) {
    SList list;
    list.reverse();
    ASSERT_EQ(0, list.size());
    ASSERT_EQ(IntVec(), list.toVector());
}

TEST(single_node) {
    SList list{7};
    list.reverse();
    ASSERT_EQ(IntVec({7}), list.toVector());
}

TEST(two_nodes) {
    SList list{1, 2};
    list.reverse();
    ASSERT_EQ(IntVec({2, 1}), list.toVector());
}

TEST(three_nodes) {
    SList list{1, 2, 3};
    list.reverse();
    ASSERT_EQ(IntVec({3, 2, 1}), list.toVector());
    ASSERT_EQ("[3, 2, 1]", list.toString());
}

TEST(many_nodes) {
    IntVec values;
    for (int i = 0; i < 1000; i++) values.push_back(i * 7 % 13);
    SList list(values);
    list.reverse();
    ASSERT_EQ(reversed(values), list.toVector());
}

TEST(size_unchanged) {
    SList list{1, 2, 3, 4, 5};
    list.reverse();
    ASSERT_EQ(5, list.size());
}

TEST(reverse_twice_restores_original) {
    SList list{4, 8, 15, 16, 23, 42};
    list.reverse();
    list.reverse();
    ASSERT_EQ(IntVec({4, 8, 15, 16, 23, 42}), list.toVector());
}

TEST(relinks_the_same_nodes) {
    SList list{1, 2, 3, 4, 5};
    AddrVec before = list.nodeAddresses();
    list.reverse();
    ASSERT_EQ_MSG(reversed(before), list.nodeAddresses(),
                  "expected the SAME nodes in reverse order -- re-link the "
                  "nodes, don't copy/swap data or allocate new ones");
}

TEST(creates_and_deletes_no_nodes) {
    SList list{1, 2, 3, 4, 5, 6};
    int before = nodes();
    list.reverse();
    ASSERT_EQ(before, nodes());
}

TEST(old_front_becomes_last_with_null_next) {
    /* toVector throws "broken list" if the old front's next still points
       at the second node (that would create a cycle). */
    SList list{1, 2};
    list.reverse();
    ASSERT_NO_THROW(list.toVector());
    list.pushAtBack(0); /* walks to the end, so the end must be correct */
    ASSERT_EQ(IntVec({2, 1, 0}), list.toVector());
}

TEST(list_usable_afterwards) {
    SList list{1, 2, 3};
    list.reverse();
    list.pushAtFront(4);
    list.pushAtBack(0);
    ASSERT_EQ(IntVec({4, 3, 2, 1, 0}), list.toVector());
    list.reverse();
    ASSERT_EQ(IntVec({0, 1, 2, 3, 4}), list.toVector());
}

TEST(memory_no_leaks_after_destruction) {
    int before = nodes();
    {
        SList list{1, 2, 3, 4};
        list.reverse();
    }
    ASSERT_EQ(before, nodes());
}

TEST(performance_1000000_nodes_is_linear) {
    IntVec values;
    for (int i = 0; i < 1000000; i++) values.push_back(i);
    SList list(values);
    list.reverse();
    IntVec result = list.toVector();
    ASSERT_EQ(1000000, static_cast<int>(result.size()));
    ASSERT_EQ(999999, result.front());
    ASSERT_EQ(0, result.back());
    ASSERT_EQ(500000, result[499999]);
}
