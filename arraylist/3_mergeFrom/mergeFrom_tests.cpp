/*
 * mergeFrom_tests.cpp -- tests for PROBLEM A3 (NumList::mergeFrom).
 *
 * Run all:   make al3
 */

#include "NumList.h"
#include "test_framework.h"

#include <algorithm>
#include <cstdlib>
#include <vector>

using IntVec = std::vector<int>;
using testing::liveArrayCount;
using testing::totalArrayAllocations;

static IntVec sortedRandom(int n, int maxValue) {
    IntVec v;
    for (int i = 0; i < n; i++) v.push_back(std::rand() % maxValue - maxValue / 2);
    std::sort(v.begin(), v.end());
    return v;
}

static IntVec merged(const IntVec &a, const IntVec &b) {
    IntVec out(a.size() + b.size());
    std::merge(a.begin(), a.end(), b.begin(), b.end(), out.begin());
    return out;
}

TEST(both_empty) {
    NumList a, b;
    a.mergeFrom(b);
    ASSERT_EQ(0, a.size());
    ASSERT_EQ(0, b.size());
}

TEST(this_empty_other_nonempty) {
    NumList a;
    NumList b{1, 2, 3};
    a.mergeFrom(b);
    ASSERT_EQ(IntVec({1, 2, 3}), a.toVector());
    ASSERT_EQ(IntVec({1, 2, 3}), b.toVector());
}

TEST(this_nonempty_other_empty) {
    NumList a{1, 2, 3};
    NumList b;
    a.mergeFrom(b);
    ASSERT_EQ(IntVec({1, 2, 3}), a.toVector());
}

TEST(example_from_spec) {
    NumList a{1, 4, 9};
    NumList b{2, 3, 10};
    a.mergeFrom(b);
    ASSERT_EQ(IntVec({1, 2, 3, 4, 9, 10}), a.toVector());
    ASSERT_EQ(IntVec({2, 3, 10}), b.toVector());
}

TEST(perfectly_interleaved) {
    NumList a{1, 3, 5, 7};
    NumList b{2, 4, 6, 8};
    a.mergeFrom(b);
    ASSERT_EQ(IntVec({1, 2, 3, 4, 5, 6, 7, 8}), a.toVector());
}

TEST(all_of_other_comes_first) {
    NumList a{10, 20};
    NumList b{1, 2, 3};
    a.mergeFrom(b);
    ASSERT_EQ(IntVec({1, 2, 3, 10, 20}), a.toVector());
}

TEST(all_of_other_comes_last) {
    NumList a{1, 2, 3};
    NumList b{10, 20};
    a.mergeFrom(b);
    ASSERT_EQ(IntVec({1, 2, 3, 10, 20}), a.toVector());
}

TEST(other_fits_in_the_middle) {
    NumList a{1, 100};
    NumList b{5, 6, 7};
    a.mergeFrom(b);
    ASSERT_EQ(IntVec({1, 5, 6, 7, 100}), a.toVector());
}

TEST(duplicates_within_and_across_lists) {
    NumList a{1, 1, 2, 5, 5};
    NumList b{1, 2, 2, 5};
    a.mergeFrom(b);
    ASSERT_EQ(IntVec({1, 1, 1, 2, 2, 2, 5, 5, 5}), a.toVector());
}

TEST(negative_numbers) {
    NumList a{-10, -3, 0};
    NumList b{-7, -3, 4};
    a.mergeFrom(b);
    ASSERT_EQ(IntVec({-10, -7, -3, -3, 0, 4}), a.toVector());
}

TEST(size_is_sum_and_capacity_is_enough) {
    NumList a{1, 2, 3, 4, 5};
    NumList b{1, 2, 3, 4, 5, 6, 7};
    a.mergeFrom(b);
    ASSERT_EQ(12, a.size());
    ASSERT_TRUE(a.capacity() >= a.size());
}

TEST(other_is_unchanged) {
    NumList a{2, 4};
    NumList b{1, 3, 5};
    a.mergeFrom(b);
    ASSERT_EQ(IntVec({1, 3, 5}), b.toVector());
    ASSERT_EQ(3, b.size());
}

TEST(result_is_independent_of_other) {
    NumList a{2, 4};
    NumList b{1, 3, 5};
    a.mergeFrom(b);
    b.pushAtBack(6);
    ASSERT_EQ(IntVec({1, 2, 3, 4, 5}), a.toVector());
}

TEST(list_still_usable_afterwards) {
    NumList a{2, 4};
    NumList b{1, 3};
    a.mergeFrom(b);
    for (int i = 5; i < 20; i++) a.pushAtBack(i);
    ASSERT_EQ(19, a.size());
    ASSERT_EQ(19, a.elementAt(18));
    ASSERT_EQ(1, a.elementAt(0));
}

TEST(merge_twice) {
    NumList a{5};
    NumList b{1, 9};
    NumList c{3, 7};
    a.mergeFrom(b);
    a.mergeFrom(c);
    ASSERT_EQ(IntVec({1, 3, 5, 7, 9}), a.toVector());
}

TEST(self_merge_doubles_every_element) {
    NumList a{1, 2, 3};
    a.mergeFrom(a);
    ASSERT_EQ(IntVec({1, 1, 2, 2, 3, 3}), a.toVector());
}

TEST(self_merge_with_spare_capacity) {
    NumList a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; /* cap 30 */
    a.mergeFrom(a);
    IntVec expected;
    for (int i = 1; i <= 15; i++) { expected.push_back(i); expected.push_back(i); }
    ASSERT_EQ(expected, a.toVector());
}

TEST(memory_at_most_one_new_array_and_no_leaks) {
    NumList a{1, 3, 5};
    NumList b{2, 4, 6, 8, 10};
    int  oldCap = a.capacity();
    long totalBefore = totalArrayAllocations();
    long liveBefore = liveArrayCount();
    a.mergeFrom(b);
    ASSERT_TRUE(totalArrayAllocations() - totalBefore <= 1);
    long expectedLive = liveBefore - (oldCap > 0 ? 1 : 0) + (a.capacity() > 0 ? 1 : 0);
    ASSERT_EQ_MSG(expectedLive, liveArrayCount(),
                  "did you delete [] the old array?");
}

TEST(memory_merging_into_empty_list_no_leaks) {
    long liveBefore = liveArrayCount();
    {
        NumList a;
        NumList b{1, 2};
        a.mergeFrom(b);
        a.mergeFrom(b);
        ASSERT_EQ(IntVec({1, 1, 2, 2}), a.toVector());
    }
    ASSERT_EQ(liveBefore, liveArrayCount());
}

TEST(memory_self_merge_no_leaks) {
    long liveBefore = liveArrayCount();
    {
        NumList a{1, 2, 3};
        a.mergeFrom(a);
        a.mergeFrom(a);
        ASSERT_EQ(12, a.size());
    }
    ASSERT_EQ(liveBefore, liveArrayCount());
}

TEST(performance_100000_plus_100000_is_linear) {
    IntVec evens, odds;
    for (int i = 0; i < 100000; i++) {
        evens.push_back(2 * i);
        odds.push_back(2 * i + 1);
    }
    NumList a(evens);
    NumList b(odds);
    a.mergeFrom(b);
    ASSERT_EQ(200000, a.size());
    for (int i = 0; i < 200000; i += 997) ASSERT_EQ(i, a.elementAt(i));
    ASSERT_EQ(199999, a.elementAt(199999));
}

TEST(random_lists_match_std_merge) {
    std::srand(3);
    for (int trial = 0; trial < 300; trial++) {
        IntVec x = sortedRandom(std::rand() % 20, 30);
        IntVec y = sortedRandom(std::rand() % 20, 30);
        NumList a(x);
        NumList b(y);
        a.mergeFrom(b);
        std::string ctx = testing::show(x) + ".mergeFrom(" + testing::show(y) + ")";
        ASSERT_EQ_MSG(merged(x, y), a.toVector(), ctx);
        ASSERT_EQ_MSG(y, b.toVector(), ctx);
    }
}
