# R4: Is it a palindrome? (easy)

[Recursion problems](../README.md) | [Setup](../../README.md)

Implement `bool isPalindrome(const std::string &text)` in
[isPalindrome.cpp](isPalindrome.cpp).

## Requirements

- Return whether the characters read the same forward and backward.
  Empty and one-character strings are palindromes.
- Comparison is case-sensitive; spaces and punctuation are ordinary
  characters. Inputs are ASCII strings of at most 1000 characters.
- Compare the two endpoints, stop immediately on a mismatch, and otherwise
  recurse on the interior using indices into the same string.
- No loops, `substr`, reversed copies, or allocation. Pass strings by
  reference, not by value, including in your helper.

## Examples and edge cases

`"racecar" -> true`, `"abba" -> true`, `"abca" -> false`, `"Aa" -> false`.
Watch for empty-string index underflow and a mismatch close to the center.

## Analyze before coding

1. Write the worst-case recurrence for a string of length `n`.
2. Give tight best- and worst-case time bounds and worst-case stack space.
3. Is removing two characters per call the same as halving the size?
4. If you instead recurse on a newly copied substring, what happens to
   worst-case time and peak extra memory?

## Check your work

Run `make rec4`. Tests cover empty input, odd/even lengths, literal character
comparison, unchanged input, and mismatches at different depths.
