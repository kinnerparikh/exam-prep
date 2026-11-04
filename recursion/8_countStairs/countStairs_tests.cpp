#include "countStairs.h"
#include "test_framework.h"

TEST(base_cases) {
    ASSERT_EQ(1LL, countStairs(0));
    ASSERT_EQ(1LL, countStairs(1));
}

TEST(two_steps_have_two_ordered_paths) {
    ASSERT_EQ(2LL, countStairs(2));
}

TEST(three_and_four_steps_count_different_orders) {
    ASSERT_EQ(3LL, countStairs(3));
    ASSERT_EQ(5LL, countStairs(4));
}

TEST(small_values_match_iterative_oracle) {
    long long previous = 1;
    long long current = 1;
    for (int n = 2; n <= 18; n++) {
        long long next = previous + current;
        previous = current;
        current = next;
        ASSERT_EQ(current, countStairs(n));
    }
}

TEST(moderate_input_and_independent_calls) {
    ASSERT_EQ(121393LL, countStairs(25));
    ASSERT_EQ(8LL, countStairs(5));
    ASSERT_EQ(1LL, countStairs(0));
}
