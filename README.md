# CS15 Exam Prep: ArrayLists & LinkedLists

8 C++ practice problems: 4 on ArrayLists and 4 on LinkedLists. Each one has
starter code and a test suite. Problems **A1** and **L1** have you build the
data structure itself. The others are the "write one function" problems that
show up on exams.

```
make al1          # build + test problem A1 (do this first!)
```

## The problems

| #  | Problem | You write code in | What it practices |
|----|---------|-------------------|-------------------|
| A1 | [**IntArrayList**: build the whole class](arraylist/1_IntArrayList/README.md) | `arraylist/1_IntArrayList/IntArrayList.cpp` | dynamic arrays, `expand`, shifting, big three, `range_error`/`runtime_error` |
| A2 | [`NumList::removeAll(value)`](arraylist/2_removeAll/README.md) | `arraylist/2_removeAll/removeAll.cpp` | one-pass O(n) compaction with two indices, no extra array |
| A3 | [`NumList::mergeFrom(other)`](arraylist/3_mergeFrom/README.md) | `arraylist/3_mergeFrom/mergeFrom.cpp` | merging sorted arrays, growing capacity once, aliasing (`a.mergeFrom(a)`) |
| A4 | [**CircularArrayList** (deque)](arraylist/4_CircularArrayList/README.md) | `arraylist/4_CircularArrayList/CircularArrayList.cpp` | amortized O(1) pushes and O(1) pops at both ends, `%` wrap-around, unwrapping on `expand` |
| L1 | [**IntLinkedList**: build the whole class (doubly linked)](linkedlist/1_IntLinkedList/README.md) | `linkedlist/1_IntLinkedList/IntLinkedList.cpp` | `front`/`back`/`prev`/`next` bookkeeping, edge cases, big three, no leaks |
| L2 | [`SList::reverse()`](linkedlist/2_reverse/README.md) | `linkedlist/2_reverse/reverse.cpp` | the classic three-pointer re-link (no new nodes) |
| L3 | [`SList::removeAll(value)`](linkedlist/3_removeAll/README.md) | `linkedlist/3_removeAll/removeAll.cpp` | deleting while walking, removing at the front, consecutive matches |
| L4 | [`SList::mergeSorted(other)`](linkedlist/4_mergeSorted/README.md) | `linkedlist/4_mergeSorted/mergeSorted.cpp` | splicing two sorted lists by re-linking, stability, emptying `other` |

**Each problem now has its own README**, linked above, with the task, files to
edit, exact requirements, complexity targets, examples, edge cases, and test
commands. The original specifications also remain in the starter `.cpp`
comments and, for A1, A4, and L1, the `.h` files.

**Suggested order:** A1 → L1 → A2 → L2 → L3 → A3 → L4 → A4.
A1 and L1 come first because everything else builds on them. If you're short
on time, prioritize L1, L2, L3, and A2, since those are the patterns exams
love.

### Rules (like the exam)
- Use raw arrays/nodes with `new`/`delete`. **No** `std::vector`, `std::list`,
  or other STL containers in your implementation.
- Implement the files in the "You write code in" column. You may add private
  helper declarations in the corresponding header, but keep public interfaces,
  data members, and node definitions unchanged. Don't edit tests, `testing/`,
  or provided implementations in `common/`.
- Unimplemented non-destructor functions throw `logic_error("TODO ...")`.
  Replace that line with your implementation; also fill in empty destructor
  TODOs. Leave the functions already provided for you intact.

## Installing GNU GCC on macOS

Run from this folder:

```sh
./scripts/install-gcc-macos.sh
```

The [installer](scripts/install-gcc-macos.sh) uses
[Homebrew's GCC package](https://formulae.brew.sh/formula/gcc), updates Homebrew
before a new GCC installation, and reuses an existing GCC installation.
It verifies that a C++17 program compiles and runs with GNU GCC rather than
Apple clang, finding the versioned compiler without hard-coding a GCC version.

**Run the exact `make` command printed at the end.** It supplies `CXX` with
the installed compiler's full path and uses `BUILD=build-gcc`, keeping GNU
builds separate from clang builds. Change `al1` to another problem name or
`test` to run all problems. Use the same `CXX` and `BUILD` arguments on later
runs. `make clean` removes all three build directories.

On Macs, `/usr/bin/gcc` and `/usr/bin/g++` normally invoke Apple clang. This
script does not replace them, create aliases, or edit your shell profile.
Use the versioned GNU compiler path printed by the script. Keep the default
clang++ compiler for `make SANITIZE=1` on macOS.

The script handles Apple Silicon and 64-bit Intel Homebrew locations, but
cannot guarantee installation on every old Mac: Homebrew's current
[macOS support requirements](https://docs.brew.sh/Installation) apply.
Internet access and Apple developer tools are required. If the tools are
missing, run `xcode-select --install`, finish the installation, then rerun
the script. If Homebrew is missing, the script asks before running its
official installer in an interactive Terminal; that installer may request
your administrator password. Do not run this script with `sudo`.

## Running the tests

```
make al1                          # build + run all tests for one problem (al1..al4, ll1..ll4)
make al1 ARGS=insertAt            # only tests whose name contains "insertAt"
make al1 ARGS="expand clear"      # several filters
make arraylist                    # A1-A4, with a summary at the end
make linkedlist                   # L1-L4
make test                         # all 8, then a summary like:  al1  67/67 tests passed
make clean
```

You can also run a built program directly, e.g. `./build/ll1 --list` (lists
test names) or `./build/ll1 pop --timeout=20`.

Each test runs in its own process, so a segfault or infinite loop in one test
doesn't stop the others. Test names tell you what's being checked. Read the
failing test in `*_tests.cpp` to see exactly what it does.

| Result | Meaning |
|--------|---------|
| `PASS` | :) |
| `FAIL` | an assertion failed; shows file:line, expected vs. actual, and context |
| `CRASH` | segfault/abort (nullptr, dangling pointer, double delete, out-of-bounds...) |
| `TIMEOUT` | took > 5 s, usually an infinite loop (forgot `curr = curr->next`?) or an O(n²) algorithm on a performance test |
| `uncaught exception std::logic_error: TODO...` | you haven't implemented a function this test calls |

### Clues in the output
- **`-842150451`** (`0xCDCDCDCD`): you read array memory you never wrote
  (an uninitialized slot from `new int[n]`).
- **`-572662307`** (`0xDDDDDDDD`): you read from an array **after** you
  `delete[]`'d it (use-after-free).
- **`broken list` errors** (L2–L4): your links are wrong. `next` loops back
  (a cycle), or `numItems` doesn't match the actual number of nodes.
- **Leak checks:** tests count live arrays/nodes (`liveArrayCount`,
  `liveNodeCount`). "expected 0, actual 3" in a `memory_*` test means you
  forgot 3 `delete`s.
- **`performance_*` tests** use 100k–1M elements. An O(n²) solution will
  `TIMEOUT`.

## Debugging tools

```
make SANITIZE=1 ll1               # AddressSanitizer + UBSan build (in build-asan/)
```
Use this whenever you see a `CRASH`. ASan prints a report naming the exact
file and line for out-of-bounds accesses, use-after-free, and double deletes.
Look for the first `#N ... in` line that mentions *your* file.

On the Tufts servers (Linux), valgrind also works:
```
make valgrind-ll1 ARGS=popFromBack
```
That runs with `--no-fork`, so all tests share one process. Filter to a few
tests, because the first crash stops the run.

To use a debugger: `lldb ./build/ll1 -- --no-fork popFromBack`
(or `gdb --args ./build/ll1 --no-fork popFromBack`).

## Exam-style questions

Answer these on paper **before** coding each problem, then check yourself.

**A1 / L1: fill in the worst-case big-O**

| Operation | ArrayList | Singly linked (front only) | Doubly linked (front + back) |
|-----------|-----------|-----------|-----------|
| `elementAt(i)` | | | |
| `pushAtFront` | | | |
| `pushAtBack` | | | |
| `popFromFront` | | | |
| `popFromBack` | | | |
| `insertAt(middle)` | | | |
| `indexOf(value)` | | | |

<details><summary>Answers</summary>

| Operation | ArrayList | Singly (front only) | Doubly (front + back) |
|-----------|-----------|-----------|-----------|
| `elementAt(i)` | O(1) | O(n) | O(n) |
| `pushAtFront` | O(n) (shift) | O(1) | O(1) |
| `pushAtBack` | O(1) amortized, O(n) worst (expand) | O(n) (walk to end) | O(1) |
| `popFromFront` | O(n) (shift) | O(1) | O(1) |
| `popFromBack` | O(1) | O(n) (need the 2nd-to-last node) | O(1) |
| `insertAt(middle)` | O(n) | O(n) | O(n) |
| `indexOf(value)` | O(n) | O(n) | O(n) |

A singly linked list with a `back` pointer makes `pushAtBack` O(1), but
`popFromBack` is still O(n). Why?
</details>

- **A1:** Why grow capacity by *multiplying* instead of `cap + 1`? What is
  the total cost of n `pushAtBack` calls each way? Why does a class with a
  `new`'d array need a copy constructor, `operator=`, and destructor? What
  goes wrong with the compiler-generated ones? Why does `operator=` check for
  self-assignment?
- **A2:** What's the big-O of calling `removeAt` for every match? What input
  is the worst case?
- **A3:** Why must you be careful when `other` is `*this`? If you merge in
  place, why fill from the *back*?
- **A4:** Why can't `expand` just copy `data[0..cap-1]` into the new array?
  Why `(front - 1 + cap) % cap` and not `(front - 1) % cap` in C++?
- **L1:** List every pointer that changes when you remove a middle node from a
  doubly linked list. Which special cases need extra code (empty list, one
  node, front, back)?
- **L2:** Trace your `reverse` on `[1, 2, 3]` by drawing boxes and arrows.
  What is the space complexity of a recursive reverse, and why does it crash
  on 1,000,000 nodes?
- **L3:** Why do you need `prev` (or a `Node **`) to delete from a singly
  linked list? Why can't you do `delete curr; curr = curr->next;`?
- **L4:** Why is merging linked lists O(1) extra space, while merging arrays
  needs a new array? What does "stable" mean, and why does the tie rule
  matter?

## Layout

```
exam-prep/
├── Makefile
├── testing/test_framework.h       # tiny test framework (don't edit)
├── arraylist/
│   ├── common/NumList.{h,cpp}     # provided array-based list used by A2, A3
│   ├── 1_IntArrayList/  2_removeAll/  3_mergeFrom/  4_CircularArrayList/
└── linkedlist/
    ├── common/SList.{h,cpp}       # provided singly linked list used by L2-L4
    ├── 1_IntLinkedList/ 2_reverse/    3_removeAll/  4_mergeSorted/
```

Requires a C++17 compiler and a POSIX environment (macOS or Linux).
The Makefile defaults to `clang++`. On Linux, `make CXX=g++ al1` selects
GNU GCC when installed. On macOS, use the installer above to obtain GNU GCC
and its versioned C++ compiler path.
