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
	CXXFLAGS += -L$(LLVM_PREFIX)/lib/c++
	CXXFLAGS += -Wno-character-conversion
endif

build/bstmap_tests.o: bstmap_tests.cpp bstmap.h
	mkdir -p build && $(CXX) $(CXXFLAGS) -c $< -o $@

bstmap_tests: build/bstmap_tests.o
	$(CXX) $(CXXFLAGS) $^ -lgtest -lgmock -lgtest_main -o $@

test_bst_core: bstmap_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="BSTMapCore*"

test_bst_aug: bstmap_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="BSTMapAugmented*"

test_bst_erase: bstmap_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="BSTMapErase*"

test_bst_all: bstmap_tests
	$(ENV_VARS) ./$< --gtest_color=yes

bstmap_main: bstmap_main.cpp bstmap.h
	$(CXX) $(CXXFLAGS) bstmap_main.cpp -lgtest -lgmock -lgtest_main -o $@

run_main: bstmap_main
	$(ENV_VARS) ./$<

clean:
	rm -f bstmap_tests bstmap_main build/*
	# MacOS symbol cleanup
	rm -rf *.dSYM

.PHONY: clean run_main test_bst_core test_bst_aug test_bst_erase test_bst_all
