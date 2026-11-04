#include "editDistance.h"
#include "test_framework.h"

#include <algorithm>
#include <vector>

static int iterativeDistance(const std::string &a, const std::string &b) {
    std::vector<int> previous(b.size() + 1);
    std::vector<int> current(b.size() + 1);
    for (std::size_t j = 0; j <= b.size(); j++) previous[j] = static_cast<int>(j);
    for (std::size_t i = 1; i <= a.size(); i++) {
        current[0] = static_cast<int>(i);
        for (std::size_t j = 1; j <= b.size(); j++) {
            int replacement = previous[j - 1] + (a[i - 1] != b[j - 1]);
            current[j] = std::min({previous[j] + 1, current[j - 1] + 1,
                                   replacement});
        }
        previous.swap(current);
    }
    return previous[b.size()];
}

TEST(empty_inputs_need_no_memo_allocation) {
    long before = testing::totalArrayAllocations();
    ASSERT_EQ(0, editDistance("", ""));
    ASSERT_EQ(3, editDistance("", "abc"));
    ASSERT_EQ(3, editDistance("abc", ""));
    ASSERT_EQ(before, testing::totalArrayAllocations());
}

TEST(each_edit_operation_and_exact_matches) {
    ASSERT_EQ(0, editDistance("same", "same"));
    ASSERT_EQ(1, editDistance("cat", "cut"));
    ASSERT_EQ(1, editDistance("cat", "cart"));
    ASSERT_EQ(1, editDistance("cart", "cat"));
    ASSERT_EQ(1, editDistance("A", "a"));
}

TEST(classic_examples) {
    ASSERT_EQ(3, editDistance("kitten", "sitting"));
    ASSERT_EQ(2, editDistance("flaw", "lawn"));
    ASSERT_EQ(5, editDistance("intention", "execution"));
}

TEST(repeated_characters_and_swaps_are_not_single_edits) {
    ASSERT_EQ(2, editDistance("aaaa", "aa"));
    ASSERT_EQ(2, editDistance("ab", "ba"));
    ASSERT_EQ(1, editDistance("a b", "ab"));
}

TEST(varied_inputs_match_independent_bottom_up_oracle) {
    std::vector<std::string> words = {
        "", "a", "b", "ab", "ba", "aaa", "baba", "cab", "abcabc", "bccaa"
    };
    for (const auto &a : words) {
        for (const auto &b : words) {
            ASSERT_EQ_MSG(iterativeDistance(a, b), editDistance(a, b),
                          a + " -> " + b);
        }
    }
}

TEST(memoization_handles_many_overlapping_subproblems_without_leaks) {
    std::string a(180, 'x');
    std::string b(200, 'y');
    long live = testing::liveArrayCount();
    ASSERT_EQ(200, editDistance(a, b));
    ASSERT_EQ(live, testing::liveArrayCount());
    ASSERT_EQ(std::string(180, 'x'), a);
    ASSERT_EQ(std::string(200, 'y'), b);
    ASSERT_EQ(1, editDistance("a", "b"));
    ASSERT_EQ(0, editDistance("a", "a"));
    ASSERT_EQ(live, testing::liveArrayCount());
}
