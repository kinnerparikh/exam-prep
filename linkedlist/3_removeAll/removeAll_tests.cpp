/*
 * removeAll_tests.cpp -- tests for PROBLEM L3 (SList::removeAll).
 *
 * Run all:   make ll3
 */

#include "SList.h"
#include "test_framework.h"

#include <algorithm>
#include <cstdlib>
#include <vector>

using IntVec  = std::vector<int>;
using AddrVec = std::vector<const void *>;

static int nodes() { return SList::liveNodeCount(); }

TEST(empty_list_returns_zero) {
    SList list;
    ASSERT_EQ(0, list.removeAll(3));
    ASSERT_EQ(0, list.size());
}

TEST(value_not_present) {
    SList list{1, 2, 3};
    int before = nodes();
    ASSERT_EQ(0, list.removeAll(9));
    ASSERT_EQ(IntVec({1, 2, 3}), list.toVector());
    ASSERT_EQ(before, nodes());
}

TEST(single_node_match) {
    SList list{5};
    ASSERT_EQ(1, list.removeAll(5));
    ASSERT_EQ(0, list.size());
    ASSERT_TRUE(list.isEmpty());
    ASSERT_EQ(IntVec(), list.toVector());
}

TEST(single_node_no_match) {
    SList list{5};
    ASSERT_EQ(0, list.removeAll(4));
    ASSERT_EQ(IntVec({5}), list.toVector());
}

TEST(remove_front) {
    SList list{9, 1, 2};
    ASSERT_EQ(1, list.removeAll(9));
    ASSERT_EQ(IntVec({1, 2}), list.toVector());
}

TEST(remove_several_at_front) {
    SList list{9, 9, 9, 1, 2};
    ASSERT_EQ(3, list.removeAll(9));
    ASSERT_EQ(IntVec({1, 2}), list.toVector());
}

TEST(remove_back) {
    SList list{1, 2, 9};
    ASSERT_EQ(1, list.removeAll(9));
    ASSERT_EQ(IntVec({1, 2}), list.toVector());
    list.pushAtBack(3); /* the new last node's next must be nullptr */
    ASSERT_EQ(IntVec({1, 2, 3}), list.toVector());
}

TEST(remove_middle) {
    SList list{1, 9, 2};
    ASSERT_EQ(1, list.removeAll(9));
    ASSERT_EQ(IntVec({1, 2}), list.toVector());
}

TEST(consecutive_matches_in_middle) {
    SList list{1, 9, 9, 9, 2};
    ASSERT_EQ(3, list.removeAll(9));
    ASSERT_EQ(IntVec({1, 2}), list.toVector());
}

TEST(consecutive_matches_at_back) {
    SList list{1, 2, 9, 9};
    ASSERT_EQ(2, list.removeAll(9));
    ASSERT_EQ(IntVec({1, 2}), list.toVector());
}

TEST(example_from_spec) {
    SList list{4, 4, 1, 4, 2, 4};
    ASSERT_EQ(4, list.removeAll(4));
    ASSERT_EQ(IntVec({1, 2}), list.toVector());
}

TEST(every_node_matches) {
    SList list{3, 3, 3, 3};
    ASSERT_EQ(4, list.removeAll(3));
    ASSERT_EQ(0, list.size());
    ASSERT_EQ(IntVec(), list.toVector());
    list.pushAtFront(1); /* front must be nullptr so this works */
    ASSERT_EQ(IntVec({1}), list.toVector());
}

TEST(alternating_matches) {
    SList list{0, 1, 0, 2, 0, 3, 0};
    ASSERT_EQ(4, list.removeAll(0));
    ASSERT_EQ(IntVec({1, 2, 3}), list.toVector());
}

TEST(negative_values) {
    SList list{-1, 5, -1, -2};
    ASSERT_EQ(2, list.removeAll(-1));
    ASSERT_EQ(IntVec({5, -2}), list.toVector());
}

TEST(removed_nodes_are_deleted) {
    SList list{1, 2, 1, 3, 1};
    int before = nodes();
    ASSERT_EQ(3, list.removeAll(1));
    ASSERT_EQ_MSG(before - 3, nodes(), "every removed node must be deleted");
}

TEST(kept_nodes_are_the_original_nodes) {
    SList list{7, 1, 7, 2, 7, 3, 7};
    AddrVec all = list.nodeAddresses();
    AddrVec expected = {all[1], all[3], all[5]};
    list.removeAll(7);
    ASSERT_EQ_MSG(expected, list.nodeAddresses(),
                  "keep the original nodes -- don't rebuild the list");
}

TEST(calling_twice) {
    SList list{1, 2, 1};
    ASSERT_EQ(2, list.removeAll(1));
    ASSERT_EQ(0, list.removeAll(1));
    ASSERT_EQ(IntVec({2}), list.toVector());
}

TEST(memory_no_leaks_after_destruction) {
    int before = nodes();
    {
        SList list{1, 2, 1, 2, 1};
        list.removeAll(2);
        list.removeAll(1);
        list.pushAtBack(5);
    }
    ASSERT_EQ(before, nodes());
}

TEST(random_lists_match_std_remove) {
    std::srand(5);
    for (int trial = 0; trial < 300; trial++) {
        IntVec values;
        int n = std::rand() % 25;
        for (int i = 0; i < n; i++) values.push_back(std::rand() % 3);
        int target = std::rand() % 3;

        int before = nodes();
        SList list(values);
        IntVec expected = values;
        expected.erase(std::remove(expected.begin(), expected.end(), target),
                       expected.end());
        int removed = list.removeAll(target);

        std::string ctx = "removeAll(" + std::to_string(target) + ") on " +
                          testing::show(values);
        ASSERT_EQ_MSG(static_cast<int>(values.size() - expected.size()),
                      removed, ctx);
        ASSERT_EQ_MSG(expected, list.toVector(), ctx);
        ASSERT_EQ_MSG(before + static_cast<int>(expected.size()), nodes(), ctx);
    }
}
