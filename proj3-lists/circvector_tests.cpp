#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "circvector.h"

using namespace std;
using namespace testing;

// Test default constructor
TEST(CircVectorCore, DefaultConstructor) {
  CircVector<int> vec;
  EXPECT_TRUE(vec.empty());
  EXPECT_EQ(vec.size(), 0);
  EXPECT_EQ(vec.get_capacity(), 10);
}

// Test constructor with custom capacity
TEST(CircVectorCore, ConstructorWithCapacity) {
  CircVector<int> vec(20);
  EXPECT_TRUE(vec.empty());
  EXPECT_EQ(vec.size(), 0);
  EXPECT_EQ(vec.get_capacity(), 20);
}

// Test push_back and pop_back
TEST(CircVectorCore, PushBackPopBack) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.pop_back(), 20);
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec.pop_back(), 10);
  EXPECT_EQ(vec.size(), 0);
  EXPECT_TRUE(vec.empty());
}

TEST(CircVectorCore, WrapBack) {
  CircVector<int> vec(5);
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);
  vec.push_back(40);
  vec.push_front(50);
  EXPECT_EQ(vec.pop_back(), 40);
}

// Test push_front and pop_front
TEST(CircVectorCore, PushFrontPopFront) {
  CircVector<int> vec;
  vec.push_front(10);
  vec.push_front(20);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.pop_front(), 20);
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec.pop_front(), 10);
  EXPECT_EQ(vec.size(), 0);
  EXPECT_TRUE(vec.empty());
}

TEST(CircVectorCore, Resize) {
  CircVector<int> vec(5);
  vec.push_front(10);
  vec.push_front(20);
  vec.push_front(30);
  vec.push_front(40);
  vec.push_front(50);
  vec.push_front(60);
  vec.push_front(70);
  vec.push_front(80);
  EXPECT_EQ(vec.size(), 8);
  EXPECT_EQ(vec.get_capacity(), 10);
  // EXPECT_EQ(vec.at(0), 60);
}

// Test at() function
TEST(CircVectorCore, AtFunction) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);
  EXPECT_EQ(vec.at(0), 10);
  EXPECT_EQ(vec.at(1), 20);
  EXPECT_EQ(vec.at(2), 30);
  EXPECT_THROW(vec.at(3), std::out_of_range);
}

// Test find function
TEST(CircVectorCore, FindFunction) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);
  EXPECT_EQ(vec.find(20), 1);
  EXPECT_EQ(vec.find(40), -1);
}

// Test pop from empty vector
TEST(CircVectorCore, PopFromEmpty) {
  CircVector<int> vec;
  EXPECT_THROW(vec.pop_front(), std::runtime_error);
  EXPECT_THROW(vec.pop_back(), std::runtime_error);
}

// Test push_back and pop_front to handle circular behavior
TEST(CircVectorCore, CircularBehavior) {
  CircVector<int> vec(3);
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec.pop_front(), 10);
  vec.push_back(40);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec.pop_front(), 20);
  EXPECT_EQ(vec.pop_front(), 30);
  EXPECT_EQ(vec.pop_front(), 40);
  EXPECT_TRUE(vec.empty());
}

// Test clear
TEST(CircVectorCore, ClearFunction) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);
  EXPECT_EQ(vec.size(), 3);
  vec.clear();
  EXPECT_EQ(vec.size(), 0);
  EXPECT_TRUE(vec.empty());
}

// Test resize function by overfilling the vector
TEST(CircVectorCore, ResizeFunction) {
  CircVector<int> vec(2);
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec.get_capacity(), 4);
}

// Part 2

// Test copy constructor
TEST(CircVectorAugmented, CopyConstructor) {
  CircVector<int> vec1;
  vec1.push_back(10);
  vec1.push_back(20);
  vec1.push_back(30);

  CircVector<int> vec2(vec1);
  EXPECT_EQ(vec2.size(), 3);
  EXPECT_EQ(vec2.at(0), 10);
  EXPECT_EQ(vec2.at(1), 20);
  EXPECT_EQ(vec2.at(2), 30);

  EXPECT_EQ(vec2.get_capacity(), vec1.get_capacity());

  CircVector<int> emptyVec;
  CircVector<int> copyEmpty(emptyVec);
  EXPECT_EQ(copyEmpty.size(), 0);
}

// Test assignment operator
TEST(CircVectorAugmented, AssignmentOperator) {
  CircVector<int> vec1;
  vec1.push_back(10);
  vec1.push_back(20);
  vec1.push_back(30);

  CircVector<int> vec2;
  vec2 = vec1;

  EXPECT_EQ(vec2.size(), 3);
  EXPECT_EQ(vec2.at(0), 10);
  EXPECT_EQ(vec2.at(1), 20);
  EXPECT_EQ(vec2.at(2), 30);
  EXPECT_EQ(vec2.get_capacity(), vec1.get_capacity());

  CircVector<int> emptyVec;
  vec2 = emptyVec;
  EXPECT_EQ(vec2.size(), 0);

  vec2.push_back(5);
  CircVector<int>& ref = vec2;
  vec2 = ref;

  EXPECT_EQ(vec2.size(), 1);
  EXPECT_EQ(vec2.at(0), 5);
}

// Test to_string function
TEST(CircVectorAugmented, ToString) {
  CircVector<int> vec;
  EXPECT_EQ(vec.to_string(), "[]");

  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);
  EXPECT_EQ(vec.to_string(), "[10, 20, 30]");
}

// Test find function
TEST(CircVectorAugmented, Find) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);
  EXPECT_EQ(vec.find(20), 1);
  EXPECT_EQ(vec.find(40), -1);

  CircVector<int> emptyVec;
  EXPECT_EQ(emptyVec.find(10), -1);
}

// Test remove_at function
TEST(CircVectorAugmented, RemoveAt) {
  CircVector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);

  vec.remove_at(1);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.at(0), 10);
  EXPECT_EQ(vec.at(1), 30);

  vec.remove_at(0);
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec.at(0), 30);

  vec.remove_at(0);
  EXPECT_EQ(vec.size(), 0);

  EXPECT_THROW(vec.remove_at(0), std::out_of_range);
}
