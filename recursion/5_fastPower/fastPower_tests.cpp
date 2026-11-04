#include "fastPower.h"
#include "test_framework.h"

#include <climits>

TEST(zero_exponent_including_zero_base) {
    ASSERT_EQ(1LL, fastPower(7, 0));
    ASSERT_EQ(1LL, fastPower(0, 0));
    ASSERT_EQ(1LL, fastPower(-7, 0));
}

TEST(zero_base_positive_exponent) {
    ASSERT_EQ(0LL, fastPower(0, 1));
    ASSERT_EQ(0LL, fastPower(0, 12));
}

TEST(odd_and_even_exponents) {
    ASSERT_EQ(1024LL, fastPower(2, 10));
    ASSERT_EQ(243LL, fastPower(3, 5));
    ASSERT_EQ(-32LL, fastPower(-2, 5));
    ASSERT_EQ(64LL, fastPower(-2, 6));
}

TEST(exponent_one_must_not_square_the_base) {
    ASSERT_EQ(LLONG_MAX, fastPower(LLONG_MAX, 1));
    ASSERT_EQ(LLONG_MIN, fastPower(LLONG_MIN, 1));
}

TEST(wide_results) {
    ASSERT_EQ(4611686018427387904LL, fastPower(2, 62));
    ASSERT_EQ(-9223372036854775807LL - 1, fastPower(-2, 63));
}

TEST(huge_exponent_reuses_the_half_result) {
    ASSERT_EQ(1LL, fastPower(1, UINT_MAX));
    ASSERT_EQ(-1LL, fastPower(-1, UINT_MAX));
    ASSERT_EQ(1LL, fastPower(-1, UINT_MAX - 1));
}
