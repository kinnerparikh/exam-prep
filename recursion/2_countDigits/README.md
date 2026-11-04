# R2: Count decimal digits (easy)

[Recursion problems](../README.md) | [Setup](../../README.md)

Implement `int countDigits(unsigned long long value)` in
[countDigits.cpp](countDigits.cpp).

## Requirements

- Count the decimal digits in a nonnegative integer. Zero has one digit.
- Reduce the value using integer division by 10 and recurse once.
- No loops, strings, floating-point logarithms, or extra arrays.
- All values representable by `unsigned long long` are valid.

## Examples and edge cases

`0 -> 1`, `9 -> 1`, `10 -> 2`, `1002003 -> 7`.
Check both sides of a power of ten and values too large for signed `int`.

## Analyze before coding

1. Let `v` be the numeric value. Write the recurrence, including the base.
2. Give the tight time and stack bounds in terms of `v`, then in terms of
   the number of decimal digits `d`. State how you handle `v == 0`.
3. Why is "one recursive call" not enough to conclude linear time in `v`?

Use the unit-cost arithmetic model described in the recursion overview.
Although the C++ type has a fixed limit, analyze how the number of calls
grows with the value rather than answering "every machine value is bounded."

## Check your work

Run `make rec2`. Tests cover zero, single digits, power-of-ten boundaries,
internal zeroes, and a 20-digit unsigned value.
