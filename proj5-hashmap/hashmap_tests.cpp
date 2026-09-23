#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "hashmap.h"

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

TEST(FakeTest, PleaseDeleteOnceYouWriteSome) {
  // If no tests exist, GoogleTest fails to compile with a fairly confusing
  // message. We have this empty test that does nothing to allow an empty
  // autograder submission to compile and regularly fail. Once you write your
  // own tests for the project, you can delete this one.
  EXPECT_THAT(1, Eq(1));
}
TEST(HashMapCore, InsertTest) {
  HashMap<int, string> map;
  map.insert(0, "zero");
  map.insert(1, "one");
  map.insert(2, "two");
}

TEST(HashMapCore, EraseTest) {
  HashMap<string, int> map;
  map.insert("apple", 5);
  map.insert("banana", 10);

  EXPECT_EQ(map.erase("apple"), 5);
  EXPECT_THROW(map.at("apple"), std::out_of_range);
  EXPECT_EQ(map.size(), 1);
}

TEST(HashMapCore, ContainsTest) {
  HashMap<string, int> map;
  map.insert("apple", 5);

  EXPECT_TRUE(map.contains("apple"));
  EXPECT_FALSE(map.contains("banana"));
}

TEST(HashMapCore, AtTest) {
  HashMap<string, int> map;
  map.insert("apple", 5);
  map.insert("banana", 10);

  EXPECT_EQ(map.at("apple"), 5);
  EXPECT_EQ(map.at("banana"), 10);
  EXPECT_THROW(map.at("cherry"), std::out_of_range);
}

TEST(HashMapCore, ClearTest) {
  HashMap<string, int> map;
  map.insert("apple", 5);
  map.insert("banana", 10);

  map.clear();
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(map.size(), 0);
}

TEST(HashMapCore, CopyConstructorTest) {
  HashMap<string, int> map;
  map.insert("apple", 5);
  map.insert("banana", 10);

  HashMap<string, int> copy(map);

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(copy.at("apple"), 5);
  EXPECT_EQ(copy.at("banana"), 10);
}

TEST(HashMapCore, AssignmentOperatorTest) {
  HashMap<string, int> map;
  map.insert("apple", 5);
  map.insert("banana", 10);

  HashMap<string, int> copy;
  copy = map;

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(copy.at("apple"), 5);
  EXPECT_EQ(copy.at("banana"), 10);
}

TEST(HashMapCore, SelfAssignmentTest) {
  HashMap<string, int> map;
  map.insert("apple", 5);
  map.insert("banana", 10);

  HashMap<string, int> copy(map);
  copy = map;
  EXPECT_EQ(map.size(), 2);
  EXPECT_EQ(map.at("apple"), 5);
  EXPECT_EQ(map.at("banana"), 10);
}

TEST(HashMapCore, EmptyTrueTest) {
  HashMap<string, int> map;

  EXPECT_TRUE(map.empty());

  map.insert("apple", 5);
  EXPECT_FALSE(map.empty());

  map.erase("apple");
  EXPECT_TRUE(map.empty());
}

TEST(HashMapCore, InsertDoesNotOverwriteTest) {
  HashMap<string, int> map;

  map.insert("apple", 1);
  map.insert("apple", 2);

  EXPECT_EQ(map.size(), 1);
  EXPECT_EQ(map.at("apple"), 1);
}
// error
TEST(HashMapCore, InsertHandlesCollisionCorrectly) {
  HashMap<string, int> map;

  map.insert("Ea", 1);
  map.insert("FB", 2);

  EXPECT_EQ(map.size(), 2);
  EXPECT_EQ(map.at("Ea"), 1);
  EXPECT_EQ(map.at("FB"), 2);
}
// error
TEST(HashMapCore, AtWrongValueCollisionTest) {
  HashMap<string, int> map;

  map.insert("keyA", 10);
  map.insert("keyB", 20);

  EXPECT_EQ(map.at("keyA"), 10);
  EXPECT_EQ(map.at("keyB"), 20);
  EXPECT_THROW(map.at("nonexistent"), std::out_of_range);
}

TEST(HashMapCore, AssignSelfDestructiveTest) {
  HashMap<string, int> map;
  map.insert("a", 1);
  map.insert("b", 2);

  HashMap<string, int>* ptr = &map;
  *ptr = *ptr;

  EXPECT_EQ(map.size(), 2);
  EXPECT_EQ(map.at("a"), 1);
  EXPECT_EQ(map.at("b"), 2);
}

TEST(HashMapCore, EraseThrowsIfKeyNotFound) {
  HashMap<string, int> map;
  map.insert("apple", 5);
  EXPECT_THROW(map.erase("banana"), std::out_of_range);
}

TEST(HashMapCore, EraseOnlyRemovesOthers) {
  HashMap<string, int> map;
  map.insert("Ea", 1);
  map.insert("FB", 2);
  map.erase("Ea");
  EXPECT_TRUE(map.contains("FB"));
  EXPECT_FALSE(map.contains("Ea"));
  EXPECT_EQ(map.at("FB"), 2);
}
TEST(HashMapCore, EaFBForceCollision) {
  HashMap<string, int> map(2);
  map.insert("Ea", 10);
  map.insert("FB", 20);
  EXPECT_TRUE(map.contains("Ea"));
  EXPECT_TRUE(map.contains("FB"));
  EXPECT_EQ(map.at("Ea"), 10);
  EXPECT_EQ(map.at("FB"), 20);
}
TEST(HashMapCore, TEst) {
  HashMap<int, string> map1;
  map1.insert(1, "one");
  map1.insert(2, "two");

  HashMap<int, string> map2;
  map2.insert(100, "old");
  map2 = map1;

  EXPECT_THAT(map2.size(), Eq(2));
  EXPECT_TRUE(map2.contains(1));
  EXPECT_TRUE(map2.contains(2));
  EXPECT_FALSE(map2.contains(100));
}
TEST(HashMapCore, EraseDoesNotRemoveOtherCollidedKeys) {
  HashMap<string, int> map(1);  // all in same bucket

  map.insert("foo", 10);
  map.insert("bar", 20);

  map.erase("foo");

  EXPECT_FALSE(map.contains("foo"));
  EXPECT_TRUE(map.contains("bar"));
  EXPECT_EQ(map.at("bar"), 20);
}
TEST(HashMapCore, AtWrongValueInCollisionTest) {
  HashMap<string, int> map(1);  // all keys collide
  map.insert("one", 100);
  map.insert("two", 200);

  EXPECT_EQ(map.at("one"), 100);
  EXPECT_EQ(map.at("two"), 200);
}
TEST(HashMapCore, InsertCollisionBugTest) {
  HashMap<string, int> map(1);  // force all keys into the same bucket

  map.insert("key1", 1);
  map.insert("key2", 2);

  EXPECT_EQ(map.size(), 2);
  EXPECT_EQ(map.at("key1"), 1);
  EXPECT_EQ(map.at("key2"), 2);
}
TEST(HashMapCore, EraseRemovesOnlyTargetKeyInCollision) {
  // Create a map with a small bucket size to force collisions
  HashMap<string, int> map(1);  // Only 1 bucket, everything collides

  map.insert("key1", 100);  // Insert two keys that will collide
  map.insert("key2", 200);

  // Ensure both are inserted correctly
  EXPECT_EQ(map.size(), 2);
  EXPECT_EQ(map.at("key1"), 100);
  EXPECT_EQ(map.at("key2"), 200);

  // Erase "key1" and check that "key2" is not removed
  EXPECT_EQ(map.erase("key1"), 100);  // erase "key1"
  EXPECT_FALSE(map.contains("key1"));
  EXPECT_TRUE(map.contains("key2"));  // "key2" should still exist
  EXPECT_EQ(map.at("key2"), 200);     // "key2" should remain unaffected
}

// Part 2

TEST(HashMapAugmented, EqualityCheckEmptyMaps) {
  HashMap<int, string> map1;
  HashMap<int, string> map2;
  EXPECT_TRUE(map1 == map2);
}

TEST(HashMapAugmented, EqualityCheckSameContents) {
  HashMap<string, int> map1;
  map1.insert("a", 1);
  map1.insert("b", 2);

  HashMap<string, int> map2;
  map2.insert("b", 2);
  map2.insert("a", 1);

  EXPECT_TRUE(map1 == map2);
}

TEST(HashMapAugmented, InequalityCheckDifferentSize) {
  HashMap<string, int> map1;
  map1.insert("a", 1);

  HashMap<string, int> map2;
  map2.insert("a", 1);
  map2.insert("b", 2);

  EXPECT_FALSE(map1 == map2);
}

TEST(HashMapAugmented, InequalityCheckDifferentValues) {
  HashMap<string, int> map1;
  map1.insert("a", 1);
  map1.insert("b", 2);

  HashMap<string, int> map2;
  map2.insert("a", 1);
  map2.insert("b", 3);

  EXPECT_FALSE(map1 == map2);
}

TEST(HashMapAugmented, InequalityCheckDifferentKeys) {
  HashMap<string, int> map1;
  map1.insert("a", 1);
  map1.insert("b", 2);

  HashMap<string, int> map2;
  map2.insert("a", 1);
  map2.insert("c", 2);

  EXPECT_FALSE(map1 == map2);
}

TEST(HashMapAugmented, EqualityCheckDifferentCapacitySameContent) {
  HashMap<string, int> map1(10);
  map1.insert("x", 100);

  HashMap<string, int> map2(2);
  map2.insert("x", 100);

  EXPECT_TRUE(map1 == map2);
}

TEST(HashMapAugmented, BeginNextOnEmptyMap) {
  HashMap<int, int> map;
  int key, value;
  map.begin();
  EXPECT_FALSE(map.next(key, value));
}

TEST(HashMapAugmented, BeginNextSingleElement) {
  HashMap<int, string> map;
  map.insert(5, "five");
  int key;
  string value;
  map.begin();
  EXPECT_TRUE(map.next(key, value));
  EXPECT_EQ(key, 5);
  EXPECT_EQ(value, "five");
  EXPECT_FALSE(map.next(key, value));
}

TEST(HashMapAugmented, BeginNextMultipleElementsNoCollision) {
  HashMap<int, int> map(10);
  map.insert(1, 10);
  map.insert(5, 50);
  map.insert(9, 90);

  set<pair<int, int>> expected = {{1, 10}, {5, 50}, {9, 90}};
  set<pair<int, int>> actual;
  int key, value;

  map.begin();
  while (map.next(key, value)) {
    actual.insert({key, value});
  }

  EXPECT_EQ(actual, expected);
  EXPECT_EQ(actual.size(), 3);
}

TEST(HashMapAugmented, BeginNextHandlesCollision) {
  HashMap<string, int> map(1);
  map.insert("key1", 10);
  map.insert("key2", 20);
  map.insert("key3", 30);

  set<pair<string, int>> expected = {{"key1", 10}, {"key2", 20}, {"key3", 30}};
  set<pair<string, int>> actual;
  string key;
  int value;

  map.begin();
  while (map.next(key, value)) {
    actual.insert({key, value});
  }

  EXPECT_EQ(actual, expected);
  EXPECT_EQ(actual.size(), 3);
}

TEST(HashMapAugmented, BeginResetsIteration) {
  HashMap<int, int> map(2);
  map.insert(1, 10);
  map.insert(5, 50);

  int key, value;
  int count = 0;

  map.begin();
  while (map.next(key, value)) {
    count++;
  }
  EXPECT_EQ(count, 2);

  count = 0;
  map.begin();
  while (map.next(key, value)) {
    count++;
  }
  EXPECT_EQ(count, 2);
}
}  // namespace
