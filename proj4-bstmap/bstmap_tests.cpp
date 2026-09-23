#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "bstmap.h"

using namespace std;
using namespace testing;

namespace {

class Random {
 private:
  static mt19937 rng;

 public:
  static void seed(int s) {
    Random::rng.seed(s);
  }

  /**
   * Generate a random integer in the range 0 (inclusive) to `max` (exclusive)
   */
  static int randInt(int max) {
    // Even though mt19937 is standardized, STL distributions aren't!
    // So, unfortunately, even though this is biased, we're forced to
    // do something like this. Technically uint32_fast_t isn't...
    // totally consistent across platforms? But within reason it works.
    return rng() % (max + 1);
  }
};

std::mt19937 Random::rng;

// Part 1
TEST(BSTMapCore, EmptyOnInitialization) {
  BSTMap<int, string> bst;
  EXPECT_TRUE(bst.empty());
  EXPECT_EQ(bst.size(), 0);
}

TEST(BSTMapCore, InsertAndContains) {
  BSTMap<int, string> bst;
  bst.insert(10, "Ten");
  bst.insert(5, "Five");
  bst.insert(15, "Fifteen");

  EXPECT_FALSE(bst.empty());
  EXPECT_EQ(bst.size(), 3);

  EXPECT_TRUE(bst.contains(10));
  EXPECT_TRUE(bst.contains(5));
  EXPECT_TRUE(bst.contains(15));
  EXPECT_FALSE(bst.contains(7));
}

TEST(BSTMapCore, AtThrowsWhenKeyAbsent) {
  BSTMap<int, string> bst;
  bst.insert(20, "Twenty");
  EXPECT_THROW(bst.at(10), out_of_range);
  EXPECT_NO_THROW(bst.at(20));
  EXPECT_EQ(bst.at(20), "Twenty");
}

TEST(BSTMapCore, ClearBST) {
  BSTMap<int, string> bst;
  bst.insert(10, "Ten");
  bst.insert(15, "Fifteen");
  EXPECT_FALSE(bst.empty());

  bst.clear();
  EXPECT_TRUE(bst.empty());
  EXPECT_EQ(bst.size(), 0);
}

TEST(BSTMapCore, ToString) {
  BSTMap<string, int> names;
  names.insert("Gwen", 10);
  names.insert("Jen", 5);
  names.insert("Ben", 10);
  names.insert("Sven", 7);

  string expected = "Ben: 10\nGwen: 10\nJen: 5\nSven: 7\n";
  EXPECT_EQ(names.to_string(), expected);
}

TEST(BSTMapCore, CopyConstructor) {
  BSTMap<int, string> original;
  original.insert(1, "One");
  original.insert(2, "Two");

  BSTMap<int, string> copy(original);
  EXPECT_EQ(copy.size(), original.size());
  EXPECT_TRUE(copy.contains(1));
  EXPECT_TRUE(copy.contains(2));
  EXPECT_EQ(copy.at(1), "One");
}

TEST(BSTMapCore, AssignmentOperator) {
  BSTMap<int, string> a;
  a.insert(3, "Three");

  BSTMap<int, string> b;
  b.insert(4, "Four");

  b = a;
  EXPECT_EQ(b.size(), 1);
  EXPECT_TRUE(b.contains(3));
  EXPECT_FALSE(b.contains(4));
}

TEST(BSTMapCore, RandomizedInsertAndSize) {
  BSTMap<int, int> bst;
  Random::seed(42);

  const int numInsertions = 50;
  for (int i = 0; i < numInsertions; ++i) {
    bst.insert(Random::randInt(500), i);
  }

  EXPECT_LE(bst.size(), numInsertions);
  EXPECT_GT(bst.size(), 0);
}

TEST(BSTMapCore, CopyConstructorEmpty) {
  BSTMap<int, int> original;
  BSTMap<int, int> copy(original);
  EXPECT_TRUE(copy.empty());
  EXPECT_EQ(copy.size(), 0);
}

TEST(BSTMapCore, AssignmentOperatorEmpty) {
  BSTMap<int, int> original;
  BSTMap<int, int> copy;
  copy = original;
  EXPECT_TRUE(copy.empty());
  EXPECT_EQ(copy.size(), 0);
}

TEST(BSTMapCore, AssignmentOperatorSelfAssignment) {
  BSTMap<int, int> bst;
  bst.insert(1, 10);
  bst.operator=(bst);
  EXPECT_EQ(bst.size(), 1);
  EXPECT_TRUE(bst.contains(1));
}

TEST(BSTMapCore, AssignmentOperatorCorrectSize) {
  BSTMap<int, int> a;
  a.insert(1, 10);
  a.insert(2, 20);

  BSTMap<int, int> b;
  b.insert(3, 30);
  b = a;
  EXPECT_EQ(b.size(), 2);
  EXPECT_TRUE(b.contains(1));
  EXPECT_TRUE(b.contains(2));
  EXPECT_FALSE(b.contains(3));
}

// Part 2
TEST(BSTMapAugmented, RemoveMinThrowsOnEmpty) {
  BSTMap<int, int> bst;
  EXPECT_THROW(bst.remove_min(), runtime_error);
}

TEST(BSTMapAugmented, RemoveMinSingleElement) {
  BSTMap<int, int> bst;
  bst.insert(42, 100);
  auto result = bst.remove_min();
  EXPECT_EQ(result.first, 42);
  EXPECT_EQ(result.second, 100);
  EXPECT_TRUE(bst.empty());
}

TEST(BSTMapAugmented, RemoveMinMultipleElements) {
  BSTMap<int, string> bst;
  bst.insert(20, "twenty");
  bst.insert(10, "ten");
  bst.insert(30, "thirty");
  bst.insert(5, "five");

  auto result = bst.remove_min();
  EXPECT_EQ(result.first, 5);
  EXPECT_EQ(result.second, "five");
  EXPECT_EQ(bst.size(), 3);
  EXPECT_FALSE(bst.contains(5));
}

TEST(BSTMapAugmented, BeginAndNextInOrder) {
  BSTMap<int, string> bst;
  bst.insert(10, "a");
  bst.insert(5, "b");
  bst.insert(15, "c");
  bst.insert(12, "d");
  bst.insert(20, "e");

  vector<int> expected_keys = {5, 10, 12, 15, 20};
  vector<string> expected_vals = {"b", "a", "d", "c", "e"};

  bst.begin();
  for (size_t i = 0; i < expected_keys.size(); ++i) {
    int key;
    string val;
    EXPECT_TRUE(bst.next(key, val));
    EXPECT_EQ(key, expected_keys[i]);
    EXPECT_EQ(val, expected_vals[i]);
  }

  int dummy_key;
  string dummy_val;
  EXPECT_FALSE(bst.next(dummy_key, dummy_val));
}

TEST(BSTMapAugmented, BeginOnEmptyTree) {
  BSTMap<int, int> bst;
  bst.begin();
  int key, val;
  EXPECT_FALSE(bst.next(key, val));
}

TEST(BSTMapAugmented, EqualityNoSizeCheck) {
  BSTMap<int, string> a;
  BSTMap<int, string> b;

  a.insert(1, "one");
  a.insert(2, "two");
  b.insert(1, "one");

  EXPECT_FALSE(a == b);
}

TEST(BSTMapAugmented, EqualityOnlyKeys) {
  BSTMap<int, string> a;
  BSTMap<int, string> b;

  a.insert(1, "one");
  a.insert(2, "two");
  b.insert(2, "two");
  b.insert(1, "one");

  EXPECT_TRUE(a == b);

  b.erase(2);
  b.insert(2, "TWO");

  EXPECT_FALSE(a == b);
}

TEST(BSTMapAugmented, EqualitySizeMismatch) {
  BSTMap<int, std::string> a;
  BSTMap<int, std::string> b;

  a.insert(1, "one");
  a.insert(2, "two");
  a.insert(3, "three");

  b.insert(1, "one");
  b.insert(2, "two");

  EXPECT_FALSE(a == b);
  EXPECT_FALSE(b == a);
}

// Part 3

TEST(BSTMapErase, EraseThrowsWhenMissing) {
  BSTMap<int, int> bst;
  bst.insert(1, 2);
  EXPECT_THROW(bst.erase(999), out_of_range);
}

TEST(BSTMapErase, EraseLeafNode) {
  BSTMap<int, string> bst;
  bst.insert(10, "root");
  bst.insert(5, "left");
  bst.insert(15, "right");

  EXPECT_EQ(bst.erase(5), "left");
  EXPECT_EQ(bst.size(), 2);
  EXPECT_FALSE(bst.contains(5));
}

TEST(BSTMapErase, EraseNodeWithOneChild) {
  BSTMap<int, int> bst;
  bst.insert(10, 10);
  bst.insert(5, 5);
  bst.insert(2, 2);

  EXPECT_EQ(bst.erase(5), 5);
  EXPECT_EQ(bst.size(), 2);
  EXPECT_FALSE(bst.contains(5));
  EXPECT_TRUE(bst.contains(2));
}

TEST(BSTMapErase, EraseNodeWithTwoChildren) {
  BSTMap<int, int> bst;
  bst.insert(10, 10);
  bst.insert(5, 5);
  bst.insert(15, 15);
  bst.insert(13, 13);
  bst.insert(20, 20);

  EXPECT_EQ(bst.erase(15), 15);
  EXPECT_EQ(bst.size(), 4);
  EXPECT_FALSE(bst.contains(15));
  EXPECT_TRUE(bst.contains(13));
  EXPECT_TRUE(bst.contains(20));
}

TEST(BSTMapErase, EraseRootNodeOnly) {
  BSTMap<int, int> bst;
  bst.insert(42, 999);
  EXPECT_EQ(bst.erase(42), 999);
  EXPECT_TRUE(bst.empty());
}

TEST(BSTMapErase, EraseRootWithChildren) {
  BSTMap<int, string> bst;
  bst.insert(50, "root");
  bst.insert(30, "left");
  bst.insert(70, "right");
  bst.insert(20, "left-left");
  bst.insert(40, "left-right");

  EXPECT_EQ(bst.erase(50), "root");
  EXPECT_EQ(bst.size(), 4);
  EXPECT_FALSE(bst.contains(50));
  EXPECT_TRUE(bst.contains(30));
  EXPECT_TRUE(bst.contains(70));
}

TEST(BSTMapErase, EraseSuccessorWithChildren) {
  BSTMap<int, int> bst;
  bst.insert(10, 100);
  bst.insert(5, 50);
  bst.insert(20, 200);
  bst.insert(15, 150);
  bst.insert(17, 170);
  EXPECT_NO_THROW(bst.erase(10));
  EXPECT_EQ(bst.at(17), 170);
  EXPECT_THROW(bst.at(10), std::out_of_range);
}

TEST(BSTMapErase, EraseSuccessorNotRightChild) {
  BSTMap<int, int> bst;
  bst.insert(10, 100);
  bst.insert(5, 50);
  bst.insert(30, 300);
  bst.insert(20, 200);
  EXPECT_NO_THROW(bst.erase(10));
  EXPECT_EQ(bst.at(20), 200);
  EXPECT_THROW(bst.at(10), std::out_of_range);
}

}  // namespace
