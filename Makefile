# CS15 exam prep -- build and run the practice problems.
#
#   make al1                      build + run every test for problem A1
#   make al1 ARGS=expand          only tests whose name contains "expand"
#   make al1 ARGS="--no-fork"     any flags/filters are passed to the test program
#   make arraylist                run A1..A4
#   make linkedlist               run L1..L4
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

AL_PROBLEMS = al1 al2 al3 al4
LL_PROBLEMS = ll1 ll2 ll3 ll4
PROBLEMS    = $(AL_PROBLEMS) $(LL_PROBLEMS)

.PHONY: all test arraylist linkedlist clean $(PROBLEMS) $(addprefix valgrind-,$(PROBLEMS))

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

$(BUILD)/al1: INCLUDES = -I$(AL1)
$(BUILD)/al2 $(BUILD)/al3: INCLUDES = -I$(ALC)
$(BUILD)/al4: INCLUDES = -I$(AL4)
$(BUILD)/ll1: INCLUDES = -I$(LL1)
$(BUILD)/ll2 $(BUILD)/ll3 $(BUILD)/ll4: INCLUDES = -I$(LLC)

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

test:
	$(call run_suites,$(PROBLEMS))

clean:
	rm -rf build build-asan build-gcc
