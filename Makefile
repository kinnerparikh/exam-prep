# CS15 exam prep -- build and run the practice problems.
#
#   make al1                      build + run every test for problem A1
#   make al1 ARGS=expand          only tests whose name contains "expand"
#   make al1 ARGS="--no-fork"     any flags/filters are passed to the test program
#   make arraylist                run A1..A4
#   make linkedlist               run L1..L4
#   make rec1                     build + run recursion problem R1 (rec1..rec11)
#   make recursion                run all 11 recursion problems
#   make recursion-easy           run R1..R4 (also recursion-medium / recursion-hard)
#   make test                     run everything, then print a summary
#   make SANITIZE=1 ll1           build with AddressSanitizer+UBSan (into build-asan/)
#   make valgrind-ll1             run under valgrind (Linux, e.g. the Tufts servers)
#   make clean
#
# You can also run a built program directly:  ./build/al1 insertAt --timeout=20

CXX      = clang++
CXXFLAGS = -std=c++17 -g3 -O0 -Wall -Wextra -Wno-unused-parameter -Itesting

ifdef SANITIZE
CXXFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
BUILD     = build-asan
else
BUILD     = build
endif

FRAMEWORK = testing/test_framework.h

AL1 = arraylist/1_IntArrayList
AL2 = arraylist/2_removeAll
AL3 = arraylist/3_mergeFrom
AL4 = arraylist/4_CircularArrayList
ALC = arraylist/common
LL1 = linkedlist/1_IntLinkedList
LL2 = linkedlist/2_reverse
LL3 = linkedlist/3_removeAll
LL4 = linkedlist/4_mergeSorted
LLC = linkedlist/common
REC1 = recursion/1_sumArray
REC2 = recursion/2_countDigits
REC3 = recursion/3_binarySearch
REC4 = recursion/4_isPalindrome
REC5 = recursion/5_fastPower
REC6 = recursion/6_selectionSort
REC7 = recursion/7_mergeSort
REC8 = recursion/8_countStairs
REC9 = recursion/9_printSubsets
REC10 = recursion/10_printPermutations
REC11 = recursion/11_editDistance

AL_PROBLEMS = al1 al2 al3 al4
LL_PROBLEMS = ll1 ll2 ll3 ll4
REC_EASY    = rec1 rec2 rec3 rec4
REC_MEDIUM  = rec5 rec6 rec7 rec8
REC_HARD    = rec9 rec10 rec11
REC_PROBLEMS = $(REC_EASY) $(REC_MEDIUM) $(REC_HARD)
PROBLEMS    = $(AL_PROBLEMS) $(LL_PROBLEMS) $(REC_PROBLEMS)

.PHONY: all test arraylist linkedlist recursion recursion-easy recursion-medium recursion-hard clean $(PROBLEMS) $(addprefix valgrind-,$(PROBLEMS))

all: test

# ---- sources for each test program ----------------------------------------
$(BUILD)/al1: $(AL1)/IntArrayList.cpp $(AL1)/IntArrayList_tests.cpp $(AL1)/IntArrayList.h
$(BUILD)/al2: $(ALC)/NumList.cpp $(AL2)/removeAll.cpp $(AL2)/removeAll_tests.cpp $(ALC)/NumList.h
$(BUILD)/al3: $(ALC)/NumList.cpp $(AL3)/mergeFrom.cpp $(AL3)/mergeFrom_tests.cpp $(ALC)/NumList.h
$(BUILD)/al4: $(AL4)/CircularArrayList.cpp $(AL4)/CircularArrayList_tests.cpp $(AL4)/CircularArrayList.h
$(BUILD)/ll1: $(LL1)/IntLinkedList.cpp $(LL1)/IntLinkedList_tests.cpp $(LL1)/IntLinkedList.h
$(BUILD)/ll2: $(LLC)/SList.cpp $(LL2)/reverse.cpp $(LL2)/reverse_tests.cpp $(LLC)/SList.h
$(BUILD)/ll3: $(LLC)/SList.cpp $(LL3)/removeAll.cpp $(LL3)/removeAll_tests.cpp $(LLC)/SList.h
$(BUILD)/ll4: $(LLC)/SList.cpp $(LL4)/mergeSorted.cpp $(LL4)/mergeSorted_tests.cpp $(LLC)/SList.h
$(BUILD)/rec1: $(REC1)/sumArray.cpp $(REC1)/sumArray_tests.cpp $(REC1)/sumArray.h
$(BUILD)/rec2: $(REC2)/countDigits.cpp $(REC2)/countDigits_tests.cpp $(REC2)/countDigits.h
$(BUILD)/rec3: $(REC3)/binarySearch.cpp $(REC3)/binarySearch_tests.cpp $(REC3)/binarySearch.h
$(BUILD)/rec4: $(REC4)/isPalindrome.cpp $(REC4)/isPalindrome_tests.cpp $(REC4)/isPalindrome.h
$(BUILD)/rec5: $(REC5)/fastPower.cpp $(REC5)/fastPower_tests.cpp $(REC5)/fastPower.h
$(BUILD)/rec6: $(REC6)/selectionSort.cpp $(REC6)/selectionSort_tests.cpp $(REC6)/selectionSort.h
$(BUILD)/rec7: $(REC7)/mergeSort.cpp $(REC7)/mergeSort_tests.cpp $(REC7)/mergeSort.h
$(BUILD)/rec8: $(REC8)/countStairs.cpp $(REC8)/countStairs_tests.cpp $(REC8)/countStairs.h
$(BUILD)/rec9: $(REC9)/printSubsets.cpp $(REC9)/printSubsets_tests.cpp $(REC9)/printSubsets.h
$(BUILD)/rec10: $(REC10)/printPermutations.cpp $(REC10)/printPermutations_tests.cpp $(REC10)/printPermutations.h
$(BUILD)/rec11: $(REC11)/editDistance.cpp $(REC11)/editDistance_tests.cpp $(REC11)/editDistance.h

$(BUILD)/al1: INCLUDES = -I$(AL1)
$(BUILD)/al2 $(BUILD)/al3: INCLUDES = -I$(ALC)
$(BUILD)/al4: INCLUDES = -I$(AL4)
$(BUILD)/ll1: INCLUDES = -I$(LL1)
$(BUILD)/ll2 $(BUILD)/ll3 $(BUILD)/ll4: INCLUDES = -I$(LLC)
$(BUILD)/rec1: INCLUDES = -I$(REC1)
$(BUILD)/rec2: INCLUDES = -I$(REC2)
$(BUILD)/rec3: INCLUDES = -I$(REC3)
$(BUILD)/rec4: INCLUDES = -I$(REC4)
$(BUILD)/rec5: INCLUDES = -I$(REC5)
$(BUILD)/rec6: INCLUDES = -I$(REC6)
$(BUILD)/rec7: INCLUDES = -I$(REC7)
$(BUILD)/rec8: INCLUDES = -I$(REC8)
$(BUILD)/rec9: INCLUDES = -I$(REC9)
$(BUILD)/rec10: INCLUDES = -I$(REC10)
$(BUILD)/rec11: INCLUDES = -I$(REC11)

$(addprefix $(BUILD)/,$(PROBLEMS)): $(FRAMEWORK) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(filter %.cpp,$^)

$(BUILD):
	mkdir -p $@

# ---- running ---------------------------------------------------------------
$(PROBLEMS): %: $(BUILD)/%
	@./$(BUILD)/$@ $(ARGS) || true

valgrind-%: $(BUILD)/%
	valgrind --leak-check=full ./$(BUILD)/$* --no-fork $(ARGS) || true

# Runs each problem even if an earlier one fails (or doesn't compile),
# then prints one summary line per problem.
define run_suites
	@[ -t 1 ] && export TEST_FORCE_COLOR=1; \
	summary=""; \
	for p in $(1); do \
		printf '\n=================== %s ===================\n' $$p; \
		if $(MAKE) --no-print-directory $(BUILD)/$$p; then \
			./$(BUILD)/$$p $(ARGS) | tee $(BUILD)/$$p.log; \
			line=$$(grep -o '[0-9]*/[0-9]* tests passed' $(BUILD)/$$p.log); \
			[ -n "$$line" ] || line="test program crashed -- run 'make $$p'"; \
		else \
			line="DOES NOT COMPILE -- run 'make $$p' to see the errors"; \
		fi; \
		summary="$$summary$$(printf '\n  %-4s %s' $$p "$$line")"; \
	done; \
	printf '\n=================== summary ===================%s\n' "$$summary"
endef

arraylist:
	$(call run_suites,$(AL_PROBLEMS))

linkedlist:
	$(call run_suites,$(LL_PROBLEMS))

recursion:
	$(call run_suites,$(REC_PROBLEMS))

recursion-easy:
	$(call run_suites,$(REC_EASY))

recursion-medium:
	$(call run_suites,$(REC_MEDIUM))

recursion-hard:
	$(call run_suites,$(REC_HARD))

test:
	$(call run_suites,$(PROBLEMS))

clean:
	rm -rf build build-asan build-gcc
