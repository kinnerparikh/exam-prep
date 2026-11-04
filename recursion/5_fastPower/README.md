# R5: Exponentiation by squaring (medium)

[Recursion problems](../README.md) | [Setup](../../README.md)

Implement `long long fastPower(long long base, unsigned int exponent)` in
[fastPower.cpp](fastPower.cpp).

## Requirements

- Return `base` raised to `exponent`. Define every zero exponent, including
  `0^0`, to return `1`. Exponents are nonnegative.
- Recurse once on half the exponent, save the result, and combine it with
  multiplication. Account for an odd exponent.
- Do not use loops, `std::pow`, or a second call for the same half.
- Inputs guarantee that the result and the necessary intermediate products
  fit in `long long`. Do not compute unnecessary products: squaring
  `LLONG_MAX` when the exponent is `1` would overflow.
- Huge exponents are valid for bases such as `1` and `-1`.

## Examples and edge cases

`fastPower(2, 10) == 1024`, `fastPower(-2, 5) == -32`,
`fastPower(0, 0) == 1`. Consider zero, negative bases, odd/even exponents,
and results at the limits of `long long`.

## Analyze before coding

1. Use the exponent `e`, not the base, as the size parameter. Write the
   recurrence and tight worst-case time and stack bounds.
2. Compare with multiplying by the base and recursing on `e - 1`.
3. Suppose you write `power(base, e/2) * power(base, e/2)` instead of saving
   the half-result. Write that recurrence. Does halving still guarantee the
   same time bound?

## Check your work

Run `make rec5`. The huge-exponent case makes repeated computation or a
one-step-at-a-time approach impractical even when the answer is tiny.
