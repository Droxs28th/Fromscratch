CXX = clang++
# Unused: warn, but annoying to block compilation on
# Sign compare: noisy
# Command line arg: noisy, not relevant to students
CXXFLAGS = \
	-Wall -Wextra -Werror \
	-Wno-error=unused-function \
	-Wno-error=unused-parameter \
	-Wno-error=unused-variable \
	-Wno-error=unused-but-set-variable \
	-Wno-error=unused-value \
	-Wno-sign-compare \
	-Wno-unused-command-line-argument \
	-std=c++2a -I. -g -fno-omit-frame-pointer \
	-fsanitize=address,undefined

ENV_VARS = ASAN_OPTIONS=detect_leaks=1 LSAN_OPTIONS=suppressions=suppr.txt:print_suppressions=false

# On Ubuntu and WSL, googletest is installed to /usr/include or
# /usr/local/include, which are used by default.

# On Mac, we need to manually include them in our path. Brew installs to
# different locations on Intel/Silicon, so ask brew where things live.
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	GTEST_PREFIX := $(shell brew --prefix googletest)
	LLVM_PREFIX := $(shell brew --prefix llvm)
	CXX := $(LLVM_PREFIX)/bin/clang++
	CXXFLAGS += -I$(GTEST_PREFIX)/include
	CXXFLAGS += -L$(GTEST_PREFIX)/lib
endif

build/linkedlist_tests.o: linkedlist_tests.cpp linkedlist.h
	mkdir -p build && $(CXX) $(CXXFLAGS) -c $< -o $@

build/circvector_tests.o: circvector_tests.cpp circvector.h
	mkdir -p build && $(CXX) $(CXXFLAGS) -c $< -o $@

list_tests: build/linkedlist_tests.o build/circvector_tests.o
	$(CXX) $(CXXFLAGS) $^ -lgtest -lgmock -lgtest_main -o $@

test_ll_core: list_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="LinkedListCore*"

test_vec_core: list_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="CircVectorCore*"

test_core: list_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="LinkedListCore*:CircVectorCore*"

test_ll_aug: list_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="LinkedListAugmented*"

test_vec_aug: list_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="CircVectorAugmented*"

test_aug: list_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="LinkedListAugmented*:CircVectorAugmented*"

test_ll_extras: list_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="LinkedListExtras*"

test_vec_extras: list_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="CircVectorExtras*"

test_extras: list_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="LinkedListExtras*:CircVectorExtras*"

test_ll_all: list_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="LinkedList*"

test_vec_all: list_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="CircVector*"

test_all: list_tests
	$(ENV_VARS) ./$< --gtest_color=yes

list_main: list_main.cpp linkedlist.h circvector.h
	$(CXX) $(CXXFLAGS) list_main.cpp -lgtest -lgmock -lgtest_main -o $@

run_main: list_main
	$(ENV_VARS) ./$<

clean:
	rm -f list_tests list_main build/*
	# MacOS symbol cleanup
	rm -rf *.dSYM

.PHONY: clean run_main test_ll_core test_vec_core test_core test_ll_aug test_vec_aug test_aug test_ll_extras test_vec_extras test_extras test_ll_all test_vec_all test_all
