#include "countDigits.h"
#include "test_framework.h"

TEST(zero_has_one_digit) {
    ASSERT_EQ(1, countDigits(0));
}

TEST(single_digits) {
    ASSERT_EQ(1, countDigits(1));
    ASSERT_EQ(1, countDigits(9));
}

TEST(powers_of_ten_and_neighbors) {
    unsigned long long power = 10;
    for (int digits = 2; digits <= 19; digits++) {
        ASSERT_EQ(digits - 1, countDigits(power - 1));
        ASSERT_EQ(digits, countDigits(power));
        ASSERT_EQ(digits, countDigits(power + 1));
        power *= 10;
    }
}

TEST(internal_zeroes_count) {
    ASSERT_EQ(7, countDigits(1002003));
}

TEST(twenty_digit_unsigned_value) {
    ASSERT_EQ(20, countDigits(18446744073709551615ULL));
}
