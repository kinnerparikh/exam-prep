#include "isPalindrome.h"
#include "test_framework.h"

TEST(empty_and_single_character) {
    ASSERT_TRUE(isPalindrome(""));
    ASSERT_TRUE(isPalindrome("x"));
}

TEST(even_and_odd_palindromes) {
    ASSERT_TRUE(isPalindrome("abba"));
    ASSERT_TRUE(isPalindrome("racecar"));
    ASSERT_TRUE(isPalindrome("aa"));
}

TEST(outer_and_inner_mismatches) {
    ASSERT_TRUE(!isPalindrome("ab"));
    ASSERT_TRUE(!isPalindrome("abca"));
    ASSERT_TRUE(!isPalindrome("abcda"));
}

TEST(case_spaces_and_punctuation_are_significant) {
    ASSERT_TRUE(!isPalindrome("Aa"));
    ASSERT_TRUE(isPalindrome("a ! a"));
    ASSERT_TRUE(!isPalindrome("a a "));
}

TEST(input_is_not_changed) {
    std::string text = "level";
    ASSERT_TRUE(isPalindrome(text));
    ASSERT_EQ(std::string("level"), text);
}

TEST(long_palindrome_and_deep_mismatch) {
    std::string text(600, 'x');
    ASSERT_TRUE(isPalindrome(text));
    text[299] = 'y';
    ASSERT_TRUE(!isPalindrome(text));
}
