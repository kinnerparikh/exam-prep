/*
 * mergeSorted_tests.cpp -- tests for PROBLEM L4 (SList::mergeSorted).
 *
 * Run all:   make ll4
 */

#include "SList.h"
#include "test_framework.h"

#include <algorithm>
#include <cstdlib>
#include <utility>
#include <vector>

using IntVec  = std::vector<int>;
using AddrVec = std::vector<const void *>;

static int nodes() { return SList::liveNodeCount(); }

/* The node order a correct, STABLE merge must produce. */
static AddrVec expectedOrder(const SList &a, const SList &b) {
    using Item = std::pair<int, const void *>;
    std::vector<Item> x, y, out;
    IntVec av = a.toVector(), bv = b.toVector();
    AddrVec aa = a.nodeAddresses(), ba = b.nodeAddresses();
    for (size_t i = 0; i < av.size(); i++) x.push_back({av[i], aa[i]});
    for (size_t i = 0; i < bv.size(); i++) y.push_back({bv[i], ba[i]});
    std::merge(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(out),
               [](const Item &l, const Item &r) { return l.first < r.first; });
    AddrVec addrs;
    for (const Item &it : out) addrs.push_back(it.second);
    return addrs;
}

TEST(both_empty) {
    SList a, b;
    a.mergeSorted(b);
    ASSERT_EQ(0, a.size());
    ASSERT_EQ(0, b.size());
    ASSERT_EQ(IntVec(), a.toVector());
}

TEST(this_empty_takes_all_of_other) {
    SList a;
    SList b{1, 2, 3};
    AddrVec bNodes = b.nodeAddresses();
    a.mergeSorted(b);
    ASSERT_EQ(IntVec({1, 2, 3}), a.toVector());
    ASSERT_EQ(bNodes, a.nodeAddresses());
    ASSERT_EQ(0, b.size());
    ASSERT_EQ(IntVec(), b.toVector());
}

TEST(other_empty_changes_nothing) {
    SList a{1, 2, 3};
    SList b;
    AddrVec aNodes = a.nodeAddresses();
    a.mergeSorted(b);
    ASSERT_EQ(IntVec({1, 2, 3}), a.toVector());
    ASSERT_EQ(aNodes, a.nodeAddresses());
    ASSERT_EQ(0, b.size());
}

TEST(example_from_spec) {
    SList a{1, 4, 9};
    SList b{2, 3, 10};
    a.mergeSorted(b);
    ASSERT_EQ(IntVec({1, 2, 3, 4, 9, 10}), a.toVector());
    ASSERT_EQ(6, a.size());
    ASSERT_EQ(0, b.size());
    ASSERT_TRUE(b.isEmpty());
}

TEST(perfectly_interleaved) {
    SList a{1, 3, 5, 7};
    SList b{2, 4, 6, 8};
    a.mergeSorted(b);
    ASSERT_EQ(IntVec({1, 2, 3, 4, 5, 6, 7, 8}), a.toVector());
}

TEST(other_entirely_before_this_changes_front) {
    SList a{10, 20};
    SList b{1, 2, 3};
    a.mergeSorted(b);
    ASSERT_EQ(IntVec({1, 2, 3, 10, 20}), a.toVector());
}

TEST(other_entirely_after_this) {
    SList a{1, 2, 3};
    SList b{10, 20};
    a.mergeSorted(b);
    ASSERT_EQ(IntVec({1, 2, 3, 10, 20}), a.toVector());
    a.pushAtBack(30); /* last node's next must be nullptr */
    ASSERT_EQ(IntVec({1, 2, 3, 10, 20, 30}), a.toVector());
}

TEST(single_nodes) {
    SList a{5};
    SList b{3};
    a.mergeSorted(b);
    ASSERT_EQ(IntVec({3, 5}), a.toVector());
}

TEST(negative_numbers) {
    SList a{-10, -3, 0};
    SList b{-7, -3, 4};
    a.mergeSorted(b);
    ASSERT_EQ(IntVec({-10, -7, -3, -3, 0, 4}), a.toVector());
}

TEST(creates_and_deletes_no_nodes) {
    SList a{1, 3, 5};
    SList b{2, 4, 6, 8};
    int before = nodes();
    a.mergeSorted(b);
    ASSERT_EQ_MSG(before, nodes(), "re-link nodes; don't new or delete any");
}

TEST(moves_the_original_nodes) {
    SList a{1, 4, 9};
    SList b{2, 3, 10};
    AddrVec expected = expectedOrder(a, b);
    a.mergeSorted(b);
    ASSERT_EQ(expected, a.nodeAddresses());
}

TEST(ties_keep_this_lists_nodes_first) {
    SList a{1, 2, 2, 5};
    SList b{2, 2, 5, 6};
    AddrVec expected = expectedOrder(a, b);
    a.mergeSorted(b);
    ASSERT_EQ(IntVec({1, 2, 2, 2, 2, 5, 5, 6}), a.toVector());
    ASSERT_EQ_MSG(expected, a.nodeAddresses(),
                  "on equal values, nodes from *this must come first");
}

TEST(other_is_a_valid_empty_list_afterwards) {
    SList a{1, 3};
    SList b{2, 4};
    a.mergeSorted(b);
    ASSERT_EQ(0, b.size());
    ASSERT_EQ(IntVec(), b.toVector());
    b.pushAtBack(100); /* only works if b.front == nullptr */
    b.pushAtFront(99);
    ASSERT_EQ(IntVec({99, 100}), b.toVector());
    ASSERT_EQ(IntVec({1, 2, 3, 4}), a.toVector());
}

TEST(destroying_other_does_not_affect_this) {
    SList a{1, 3};
    int before = nodes();
    {
        SList b{2, 4};
        a.mergeSorted(b);
    } /* b's destructor runs here: it must not own any nodes anymore */
    ASSERT_EQ(before + 2, nodes());
    ASSERT_EQ(IntVec({1, 2, 3, 4}), a.toVector());
}

TEST(self_merge_does_nothing) {
    SList a{1, 2, 3};
    AddrVec aNodes = a.nodeAddresses();
    a.mergeSorted(a);
    ASSERT_EQ(IntVec({1, 2, 3}), a.toVector());
    ASSERT_EQ(aNodes, a.nodeAddresses());
    ASSERT_EQ(3, a.size());
}

TEST(merge_three_lists) {
    SList a{5, 50};
    SList b{1, 10};
    SList c{7, 70};
    a.mergeSorted(b);
    a.mergeSorted(c);
    ASSERT_EQ(IntVec({1, 5, 7, 10, 50, 70}), a.toVector());
    ASSERT_EQ(6, a.size());
}

TEST(memory_no_leaks_after_destruction) {
    int before = nodes();
    {
        SList a{1, 4};
        SList b{2, 3};
        a.mergeSorted(b);
    }
    ASSERT_EQ(before, nodes());
}

TEST(performance_300000_plus_300000_is_linear) {
    IntVec evens, odds;
    for (int i = 0; i < 300000; i++) {
        evens.push_back(2 * i);
        odds.push_back(2 * i + 1);
    }
    SList a(evens);
    SList b(odds);
    a.mergeSorted(b);
    IntVec result = a.toVector();
    ASSERT_EQ(600000, a.size());
    for (int i = 0; i < 600000; i += 1009) ASSERT_EQ(i, result[i]);
    ASSERT_EQ(599999, result.back());
}

TEST(random_lists_match_stable_merge) {
    std::srand(6);
    for (int trial = 0; trial < 300; trial++) {
        IntVec x, y;
        int n = std::rand() % 15, m = std::rand() % 15;
        for (int i = 0; i < n; i++) x.push_back(std::rand() % 10);
        for (int i = 0; i < m; i++) y.push_back(std::rand() % 10);
        std::sort(x.begin(), x.end());
        std::sort(y.begin(), y.end());

        int before = nodes();
        SList a(x);
        SList b(y);
        AddrVec expected = expectedOrder(a, b);
        a.mergeSorted(b);

        std::string ctx = testing::show(x) + ".mergeSorted(" + testing::show(y) + ")";
        ASSERT_EQ_MSG(n + m, a.size(), ctx);
        ASSERT_EQ_MSG(0, b.size(), ctx);
        ASSERT_EQ_MSG(expected, a.nodeAddresses(), ctx);
        ASSERT_EQ_MSG(before + n + m, nodes(), ctx);
    }
}
