/*
 * removeAll_tests.cpp -- tests for PROBLEM A2 (NumList::removeAll).
 *
 * Run all:   make al2
 */

#include "NumList.h"
#include "test_framework.h"

#include <algorithm>
#include <cstdlib>
#include <vector>

using IntVec = std::vector<int>;
using testing::liveArrayCount;
using testing::totalArrayAllocations;

TEST(empty_list_returns_zero) {
    NumList list;
    ASSERT_EQ(0, list.removeAll(5));
    ASSERT_EQ(0, list.size());
}

TEST(value_not_present_changes_nothing) {
    NumList list{1, 2, 3};
    ASSERT_EQ(0, list.removeAll(7));
    ASSERT_EQ(IntVec({1, 2, 3}), list.toVector());
}

TEST(single_element_match) {
    NumList list{5};
    ASSERT_EQ(1, list.removeAll(5));
    ASSERT_EQ(0, list.size());
}

TEST(single_element_no_match) {
    NumList list{5};
    ASSERT_EQ(0, list.removeAll(6));
    ASSERT_EQ(IntVec({5}), list.toVector());
}

TEST(remove_at_front) {
    NumList list{9, 1, 2, 3};
    ASSERT_EQ(1, list.removeAll(9));
    ASSERT_EQ(IntVec({1, 2, 3}), list.toVector());
}

TEST(remove_at_back) {
    NumList list{1, 2, 3, 9};
    ASSERT_EQ(1, list.removeAll(9));
    ASSERT_EQ(IntVec({1, 2, 3}), list.toVector());
}

TEST(remove_in_middle) {
    NumList list{1, 9, 2};
    ASSERT_EQ(1, list.removeAll(9));
    ASSERT_EQ(IntVec({1, 2}), list.toVector());
}

TEST(consecutive_matches) {
    NumList list{1, 9, 9, 9, 2};
    ASSERT_EQ(3, list.removeAll(9));
    ASSERT_EQ(IntVec({1, 2}), list.toVector());
}

TEST(example_from_spec) {
    NumList list{4, 1, 4, 4, 2, 4};
    ASSERT_EQ(4, list.removeAll(4));
    ASSERT_EQ(IntVec({1, 2}), list.toVector());
}

TEST(alternating_matches) {
    NumList list{0, 1, 0, 2, 0, 3, 0};
    ASSERT_EQ(4, list.removeAll(0));
    ASSERT_EQ(IntVec({1, 2, 3}), list.toVector());
}

TEST(every_element_matches) {
    NumList list{7, 7, 7, 7, 7};
    ASSERT_EQ(5, list.removeAll(7));
    ASSERT_EQ(0, list.size());
    ASSERT_EQ("[]", list.toString());
}

TEST(negative_values) {
    NumList list{-1, 2, -1, -3};
    ASSERT_EQ(2, list.removeAll(-1));
    ASSERT_EQ(IntVec({2, -3}), list.toVector());
}

TEST(preserves_relative_order_of_kept_elements) {
    NumList list{5, 0, 4, 0, 3, 0, 2, 0, 1};
    list.removeAll(0);
    ASSERT_EQ(IntVec({5, 4, 3, 2, 1}), list.toVector());
}

TEST(calling_twice_removes_nothing_the_second_time) {
    NumList list{1, 2, 1, 3};
    ASSERT_EQ(2, list.removeAll(1));
    ASSERT_EQ(0, list.removeAll(1));
    ASSERT_EQ(IntVec({2, 3}), list.toVector());
}

TEST(capacity_unchanged) {
    NumList list{1, 2, 1, 2, 1, 2, 1};
    int capBefore = list.capacity();
    list.removeAll(1);
    ASSERT_EQ(capBefore, list.capacity());
}

TEST(does_not_allocate_any_array) {
    NumList list{1, 2, 1, 2, 1, 2, 1};
    long totalBefore = totalArrayAllocations();
    long liveBefore = liveArrayCount();
    list.removeAll(1);
    ASSERT_EQ_MSG(totalBefore, totalArrayAllocations(),
                  "removeAll must work in place -- no new arrays");
    ASSERT_EQ(liveBefore, liveArrayCount());
}

TEST(list_still_usable_afterwards) {
    NumList list{3, 1, 3, 2, 3};
    list.removeAll(3);
    list.pushAtBack(3);
    list.pushAtBack(4);
    ASSERT_EQ(IntVec({1, 2, 3, 4}), list.toVector());
    ASSERT_EQ(4, list.size());
}

TEST(performance_200000_elements_is_linear) {
    IntVec values;
    for (int i = 0; i < 200000; i++) values.push_back(i % 2 == 0 ? -1 : i);
    NumList list(values);
    ASSERT_EQ(100000, list.removeAll(-1));
    ASSERT_EQ(100000, list.size());
    ASSERT_EQ(1, list.elementAt(0));
    ASSERT_EQ(199999, list.elementAt(99999));
}

TEST(random_lists_match_std_remove) {
    std::srand(2);
    for (int trial = 0; trial < 300; trial++) {
        IntVec values;
        int n = std::rand() % 30;
        for (int i = 0; i < n; i++) values.push_back(std::rand() % 4);
        int target = std::rand() % 4;

        NumList list(values);
        IntVec expected = values;
        expected.erase(std::remove(expected.begin(), expected.end(), target),
                       expected.end());
        int removed = list.removeAll(target);

        std::string ctx = "removeAll(" + std::to_string(target) + ") on " +
                          testing::show(values);
        ASSERT_EQ_MSG(static_cast<int>(values.size() - expected.size()),
                      removed, ctx);
        ASSERT_EQ_MSG(expected, list.toVector(), ctx);
    }
}
