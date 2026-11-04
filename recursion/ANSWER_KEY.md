# Recursion complexity answer key

**Answer-key branch: `kinner-branch` only.**

[Problem set and analysis assumptions](README.md)

Try each problem's analysis questions before reading its section. These
answers analyze the **required algorithm**, not just the function's name or
return value. A different correct-output implementation can have a different
complexity. The starter functions remain unimplemented on every branch.

## At a glance

All time bounds below are tight worst-case Big-O bounds: the corresponding
Theta bounds also hold for the required algorithms. The table assumes
nontrivial inputs (lengths/exponents at least 2 where a logarithm is used).
Empty/base cases take constant time unless noted. Stack bounds count live
frames, not total calls, without assuming tail-call elimination.

| Problem | Size parameter | Tight time bound | Stack | Total auxiliary space |
|---|---|---|---|---|
| R1: Array sum | `n` elements | `O(n)` | `O(n)` | `O(n)` |
| R2: Digit count | numeric value `v`, `d` decimal digits | `O(log v)` = `O(d)` | `O(d)` | `O(d)` |
| R3: Binary search | `n` elements | `O(log n)` | `O(log n)` | `O(log n)` |
| R4: Palindrome | `n` characters | `O(n)` | `O(n)` | `O(n)` |
| R5: Fast power | exponent `e` | `O(log e)` | `O(log e)` | `O(log e)` |
| R6: Selection sort | `n` elements | `O(n^2)` | `O(n)` | `O(n)` |
| R7: Merge sort | `n` elements | `O(n log n)` | `O(log n)` | `O(n)` |
| R8: Stair sequences | `n` stairs | `O(phi^n)`, `phi = (1 + sqrt(5))/2` | `O(n)` | `O(n)` |
| R9: Print subsets | `n` distinct values | `O(n * 2^n)` | `O(n)` | `O(n)` |
| R10: Print permutations | `n` distinct characters | `O(n * n!)` | `O(n)` | `O(n)` |
| R11: Memoized edit distance | nonempty lengths `m`, `n` | `O(mn)` | `O(m + n)` | `O(mn)` |

R9/R10 include all output work, but not the caller's output-storage memory.
R11 returns immediately with `O(1)` time and auxiliary space if either
string is empty. Fixed-width arithmetic and integer output use the unit-cost
model, not arbitrary-precision arithmetic.

## R1: Sum an array

Base: `T(0) = Theta(1)`. For `n > 0`:

```text
T(n) = T(n - 1) + Theta(1)
```

Each call handles one element and does one addition. Expanding gives `n`
constant-work steps, so time is `Theta(n)`. Up to `n + 1` frames are live,
giving `Theta(n)` stack space.

Splitting into two index ranges instead would give
`T(n) = 2T(n/2) + Theta(1)` for balanced inputs: still `Theta(n)` total
time, but only `Theta(log n)` live stack depth. No array copies are needed.

## R2: Count decimal digits

Base: any `0 <= v < 10` has one digit and takes constant time. Otherwise:

```text
T(v) = T(floor(v / 10)) + Theta(1)
```

Each call removes one decimal digit. For positive `v`,
`d = floor(log10(v)) + 1`, so time and stack are `Theta(d)`, equivalently
`Theta(log v)` for `v >= 10`. A bound that also handles zero and one is
`Theta(1 + log(v + 1))`. Zero has `d = 1`.

"One call per level" does not tell you how many levels exist. Dividing by
10 reaches the base much faster than subtracting one. In terms of the
input's digit count, this is linear, not logarithmic.

## R3: Binary search

Base: an empty range returns `-1` in constant time. In the worst case:

```text
T(n) = T(floor(n / 2)) + Theta(1)
```

Only one half is visited, and each call does constant work. There are
`Theta(log n)` levels, hence `Theta(log n)` worst-case time and stack space.
Finding the target at the first midpoint takes `Theta(1)` time.

Unlike R1, the problem size shrinks by a factor rather than a constant
amount. Copying the selected half at each level would change the recurrence
to `T(n) = T(n/2) + Theta(n)`. The geometric sum
`n/2 + n/4 + ...` gives `Theta(n)` worst-case time and peak copied storage,
while the number of stack frames remains logarithmic.

## R4: Palindrome check

Base: zero or one remaining character is a palindrome. For a palindrome,
or an input whose first mismatch is near the center:

```text
T(n) = T(n - 2) + Theta(1)
```

There are roughly `n/2` levels. Time and stack are both `Theta(n)`, not
logarithmic: subtracting two is not dividing by two. An immediate endpoint
mismatch gives `Theta(1)` best-case time.

Copying the interior substring at every level changes the nonrecursive
work to `Theta(n)`. The sum `n + (n-2) + (n-4) + ...` is `Theta(n^2)`.
If each caller retains its substring while the next call runs, peak string
storage is also `Theta(n^2)`. Index bounds into a shared string avoid both.

## R5: Exponentiation by squaring

Base: exponent zero returns one. With one saved half-result:

```text
T(e) = T(floor(e / 2)) + Theta(1)
```

Squaring the saved value and optionally multiplying by the base take
constant work per level. Time and stack are `Theta(log e)` for `e >= 2`,
or `Theta(1 + log(e + 1))` including zero and one. The base is not the size
parameter.

Recursing on `e - 1` instead takes `Theta(e)` time and stack. Recomputing
both identical halves gives:

```text
T(e) = 2T(floor(e / 2)) + Theta(1) = Theta(e)
```

Its stack is still `Theta(log e)`, because the two children run one after
the other, but its total number of calls is linear in `e`. Halving alone
does not determine the runtime; the number of recursive calls matters.

## R6: Recursive selection sort

Base: a range of size zero or one needs no sorting. Otherwise:

```text
T(n) = T(n - 1) + Theta(n)
```

The minimum scan makes `n - 1` comparisons, then the recursive suffix scan
makes `n - 2`, and so on. The total is `n(n - 1)/2`, giving `Theta(n^2)`
time on sorted, reversed, and arbitrary inputs for the specified algorithm.

The recursion has `Theta(n)` live frames. It uses no extra heap array, but
its total auxiliary space is **not** constant. R1 has the same one-element
reduction with constant work per call; here each call scans its range.

## R7: Merge sort

Base: ranges of size zero or one are already sorted.

```text
T(n) = T(floor(n/2)) + T(ceil(n/2)) + Theta(n)
```

Each level merges a total of `Theta(n)` elements, and there are
`Theta(log n)` levels. Time is `Theta(n log n)` even on sorted input,
because the required algorithm always performs its merges.

Only one root-to-leaf path is live at a time: `Theta(log n)` stack space.
The one shared scratch array has `Theta(n)` elements, so total auxiliary
space is `Theta(n)`.

Two calls do not automatically mean exponential time. Here their input
sizes partition the parent range; in R8, their subproblems overlap heavily.

## R8: Count stair-climbing sequences

Bases: `T(0)` and `T(1)` are constant. The required uncached recursion has:

```text
T(n) = T(n - 1) + T(n - 2) + Theta(1)
```

Ignoring the constant for the growth rate gives `x^2 = x + 1`. Its dominant
root is `phi = (1 + sqrt(5))/2`, approximately `1.618`, yielding
`Theta(phi^n)` time. `O(2^n)` is a valid but loose upper bound.

For example, the call for 5 visits 4 and 3; the call for 4 visits 3 and 2,
recomputing the entire subtree for 3. The returned answer is `F(n + 1)`
with `F(0) = 0`, `F(1) = 1`; the call count is `2F(n + 1) - 1`.

The longest path reduces the stair count by one each time, so only
`Theta(n)` stack frames are live, not exponentially many. Memoizing every
distinct stair count would reduce time to `Theta(n)`, with `Theta(n)` cache
and stack space. That is a different algorithm from the required exercise.

## R9: Print every subset

There are two choices per element: include or exclude. Ignoring printing:

```text
B(n) = 2B(n - 1) + Theta(1) = Theta(2^n)
```

The full tree has `2^n` leaves and `2^(n+1) - 1` calls. However, each of the
`n` values appears in exactly half of all subsets. For `n >= 1`, the output
therefore writes `n * 2^(n-1)` integers, plus separators and braces.
The tight time bound **including output** is `Theta(n * 2^n)`.

There is a `Theta(n)` recursion path and one `Theta(n)` selection buffer,
not a separate buffer for every subset. Total auxiliary space is
`Theta(n)`. An output stream that stores all text would itself need
`Theta(n * 2^n)` storage, but that is excluded from auxiliary space here.
The empty input still prints one empty subset in constant time.

If you only counted leaves using the same decision tree, time would be
`Theta(2^n)`. If only the count is wanted, directly evaluating `2^n` with a
valid fixed-width shift for the allowed inputs takes constant time under
the unit-cost model; enumerating the tree is unnecessary.

## R10: Print every permutation

Fix one position at a time. With `k` positions left there are `k` choices.
Keeping the original length `n` separate from the shrinking size `k`:

```text
T(k; n) = k * T(k - 1; n) + Theta(k),  k > 0
T(0; n) = Theta(n + 1)                 (write a complete line)
```

There are `n!` leaves and each prints `n` characters. Thus total time is
`Theta(n * n!)` for `n >= 1`, not just `Theta(n!)`. The tree traversal and
swaps alone take `Theta(n!)`: the number of nodes is
`sum(k=0..n, n!/(n-k)!)`, bounded by a constant times `n!`.

The shared working string and recursion path each need `Theta(n)` space.
Saving all output instead of streaming it would need `Theta(n * n!)`
storage. Empty input prints one empty line in constant time.

Undoing a swap restores the choices seen by the next sibling; without it,
later branches can skip or repeat permutations. Repeated input characters
would also cause duplicate outputs. Avoiding those duplicates needs a
per-depth uniqueness strategy, which is outside the distinct-character
contract.

## R11: Memoized edit distance

Let `D(i, j)` be the distance between suffixes `a[i..m)` and `b[j..n)`.
The recurrence for the **answer** is:

```text
D(m, j) = n - j
D(i, n) = m - i
D(i, j) = D(i+1, j+1)                         if a[i] == b[j]
D(i, j) = 1 + min(D(i+1, j),                  delete a[i]
                   D(i, j+1),                 insert b[j]
                   D(i+1, j+1))               replace a[i]
                                                otherwise
```

For time, count states: at most `(m + 1)(n + 1)` index pairs, each doing
constant work besides recursive calls. Every computed state is cached,
and each state has at most three outgoing calls. Cache hits do not expand
another subtree. Initializing the full table also costs
`Theta((m + 1)(n + 1))`. For nonempty strings, total time is `Theta(mn)`.

If both lengths are `N`, this is `Theta(N^2)`. If either string is empty,
the public function returns the other length immediately, taking `Theta(1)`
time and space; it does not allocate the table.

The table needs `Theta(mn)` space for nonempty inputs. Every recursive step
advances at least one index, so worst-case stack depth is `Theta(m + n)`,
not `Theta(mn)`. Total auxiliary space is `Theta(mn)` because the table
dominates for positive lengths.

Identical strings only follow diagonal recursive states, but initializing
the required full table still takes `Theta(mn)` time and memory. A different
allocation/caching strategy could change that best case; it is not the
specified implementation.
