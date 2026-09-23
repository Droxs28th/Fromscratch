#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "linkedlist.h"

using namespace std;
using namespace testing;

TEST(LinkedListTestCoreCore, DefaultConstructorAndEmptyList) {
  LinkedList<int> list;
  EXPECT_THAT(list.empty(), Eq(true));
  EXPECT_THAT(list.size(), Eq(0));
}

TEST(LinkedListCore, PushFront) {
  LinkedList<int> list;
  list.push_front(10);
  list.push_front(20);
  list.push_front(30);
  EXPECT_THAT(list.size(), Eq(3));
  EXPECT_THAT(list.at(0), Eq(30));
  EXPECT_THAT(list.at(1), Eq(20));
  EXPECT_THAT(list.at(2), Eq(10));
}

TEST(LinkedListCore, PushBack) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  EXPECT_THAT(list.size(), Eq(3));
  EXPECT_THAT(list.at(0), Eq(10));
  EXPECT_THAT(list.at(1), Eq(20));
  EXPECT_THAT(list.at(2), Eq(30));
}

TEST(LinkedListCore, PopFront) {
  LinkedList<int> list;
  list.push_front(10);
  list.push_front(20);
  EXPECT_THAT(list.pop_front(), Eq(20));
  EXPECT_THAT(list.size(), Eq(1));
  EXPECT_THAT(list.pop_front(), Eq(10));
  EXPECT_THAT(list.empty(), Eq(true));
  EXPECT_THROW(list.pop_front(), runtime_error);
}

TEST(LinkedListCore, PopBack) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  EXPECT_THAT(list.pop_back(), Eq(20));
  EXPECT_THAT(list.size(), Eq(1));
  EXPECT_THAT(list.pop_back(), Eq(10));
  EXPECT_THAT(list.empty(), Eq(true));
  EXPECT_THROW(list.pop_back(), runtime_error);
}

TEST(LinkedListCore, At) {
  LinkedList<int> list;
  list.push_front(10);
  list.push_front(20);
  EXPECT_THAT(list.at(0), Eq(20));
  EXPECT_THAT(list.at(1), Eq(10));
  EXPECT_THROW(list.at(2), out_of_range);
}

TEST(LinkedListCore, Clear) {
  LinkedList<int> list;
  list.push_front(10);
  list.push_front(20);
  list.clear();
  EXPECT_THAT(list.empty(), Eq(true));
  EXPECT_THAT(list.size(), Eq(0));
}

TEST(LinkedListCore, CopyConstructor) {
  LinkedList<int> list1;
  list1.push_front(10);
  list1.push_front(20);
  LinkedList<int> list2(list1);
  EXPECT_THAT(list2.size(), Eq(2));
  EXPECT_THAT(list2.at(0), Eq(20));
  EXPECT_THAT(list2.at(1), Eq(10));
}

TEST(LinkedListCore, AssignmentOperator) {
  LinkedList<int> list1;
  list1.push_front(10);
  list1.push_front(20);
  LinkedList<int> list2;
  list2 = list1;
  EXPECT_THAT(list2.size(), Eq(2));
  EXPECT_THAT(list2.at(0), Eq(20));
  EXPECT_THAT(list2.at(1), Eq(10));
}

TEST(LinkedListCore, ToString) {
  LinkedList<int> list;
  list.push_front(10);
  list.push_front(20);
  list.push_back(30);
  EXPECT_THAT(list.to_string(), Eq("[20, 10, 30]"));
}

TEST(LinkedListCore, Find) {
  LinkedList<int> list;
  list.push_front(10);
  list.push_front(20);
  list.push_back(30);
  EXPECT_THAT(list.find(10), Eq(1));
  EXPECT_THAT(list.find(20), Eq(0));
  EXPECT_THAT(list.find(30), Eq(2));
  EXPECT_THAT(list.find(40), Eq(-1));
}

TEST(LinkedListCore, RemoveAt) {
  LinkedList<int> list;
  list.push_front(10);
  list.push_front(20);
  list.push_back(30);
  list.remove_at(1);
  EXPECT_THAT(list.size(), Eq(2));
  EXPECT_THAT(list.at(0), Eq(20));
  EXPECT_THAT(list.at(1), Eq(30));
  EXPECT_THROW(list.remove_at(5), out_of_range);
}

TEST(LinkedListCore, InsertAfter) {
  LinkedList<int> list;
  list.push_front(10);
  list.push_back(20);
  list.push_back(30);
  list.insert_after(1, 25);
  EXPECT_THAT(list.size(), Eq(4));
  EXPECT_THAT(list.at(2), Eq(25));
  EXPECT_THAT(list.at(3), Eq(30));
}

TEST(LinkedListCore, RemoveEveryOther) {
  LinkedList<int> list;
  list.push_front(0);
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  list.push_front(4);
  list.remove_every_other();
  EXPECT_THAT(list.size(), Eq(3));
  EXPECT_THAT(list.at(0), Eq(4));
  EXPECT_THAT(list.at(1), Eq(2));
  EXPECT_THAT(list.at(2), Eq(0));
}

TEST(LinkedListCore, PushFrontAndBack) {
  LinkedList<int> list;
  list.push_front(10);
  list.push_back(20);
  list.push_back(30);
  list.push_front(5);
  EXPECT_THAT(list.size(), Eq(4));
  EXPECT_THAT(list.at(0), Eq(5));
  EXPECT_THAT(list.at(1), Eq(10));
  EXPECT_THAT(list.at(2), Eq(20));
  EXPECT_THAT(list.at(3), Eq(30));
}

TEST(LinkedListCore, PopFrontFromSingleElement) {
  LinkedList<int> list;
  list.push_front(10);
  EXPECT_THAT(list.pop_front(), Eq(10));
  EXPECT_THAT(list.size(), Eq(0));
  EXPECT_THAT(list.empty(), Eq(true));
}

TEST(LinkedListCore, PopBackFromSingleElement) {
  LinkedList<int> list;
  list.push_back(10);
  EXPECT_THAT(list.pop_back(), Eq(10));
  EXPECT_THAT(list.size(), Eq(0));
  EXPECT_THAT(list.empty(), Eq(true));
}

TEST(LinkedListCore, PopFrontFromEmptyList) {
  LinkedList<int> list;
  EXPECT_THROW(list.pop_front(), runtime_error);
}

TEST(LinkedListCore, PopBackFromEmptyList) {
  LinkedList<int> list;
  EXPECT_THROW(list.pop_back(), runtime_error);
}

TEST(LinkedListCore, AtOutOfRange) {
  LinkedList<int> list;
  list.push_front(10);
  list.push_back(20);
  EXPECT_THROW(list.at(2), out_of_range);
}

TEST(LinkedListCore, RemoveAtInvalidIndex) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  EXPECT_THROW(list.remove_at(2), out_of_range);
}

TEST(LinkedListCore, InsertAfterInvalidIndex) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  EXPECT_THROW(list.insert_after(2, 30), out_of_range);
}

TEST(LinkedListCore, CopyAssignFromEmptyList) {
  LinkedList<int> list1;
  LinkedList<int> list2;
  list2 = list1;
  EXPECT_THAT(list2.empty(), Eq(true));
  EXPECT_THAT(list2.size(), Eq(0));
}

TEST(LinkedListCore, RemoveEveryOtherWithSingleElement) {
  LinkedList<int> list;
  list.push_front(10);
  list.remove_every_other();
  EXPECT_THAT(list.size(), Eq(1));
  EXPECT_THAT(list.at(0), Eq(10));
}

TEST(LinkedListCore, RemoveEveryOtherWithTwoElements) {
  LinkedList<int> list;
  list.push_front(10);
  list.push_back(20);
  list.remove_every_other();
  EXPECT_THAT(list.size(), Eq(1));
  EXPECT_THAT(list.at(0), Eq(10));
}

TEST(LinkedListCore, CopyConstructorFromEmptyList) {
  LinkedList<int> list1;
  LinkedList<int> list2(list1);
  EXPECT_THAT(list2.empty(), Eq(true));
  EXPECT_THAT(list2.size(), Eq(0));
}

TEST(LinkedListCore, ToStringEmptyList) {
  LinkedList<int> list;
  EXPECT_THAT(list.to_string(), Eq("[]"));
}

TEST(LinkedListCore, ToStringSingleElement) {
  LinkedList<int> list;
  list.push_back(10);
  EXPECT_THAT(list.to_string(), Eq("[10]"));
}

TEST(LinkedListCore, ToStringMultipleElements) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  EXPECT_THAT(list.to_string(), Eq("[10, 20, 30]"));
}

// Part 2

// Copy Constructor Test
TEST(LinkedListAugmented, CopyConstructor) {
  LinkedList<int> list1;
  list1.push_back(10);
  list1.push_back(20);
  list1.push_back(30);

  LinkedList<int> list2(list1);
  EXPECT_EQ(list2.size(), 3);
  EXPECT_EQ(list2.at(0), 10);
  EXPECT_EQ(list2.at(1), 20);
  EXPECT_EQ(list2.at(2), 30);

  LinkedList<int> emptyList;
  LinkedList<int> copyEmpty(emptyList);
  EXPECT_EQ(copyEmpty.size(), 0);
}

// Assignment Operator Test
TEST(LinkedListAugmented, AssignmentOperator) {
  LinkedList<int> list1;
  list1.push_back(10);
  list1.push_back(20);
  list1.push_back(30);

  LinkedList<int> list2;
  list2 = list1;

  EXPECT_EQ(list2.size(), 3);
  EXPECT_EQ(list2.at(0), 10);
  EXPECT_EQ(list2.at(1), 20);
  EXPECT_EQ(list2.at(2), 30);

  LinkedList<int> emptyList;
  list2 = emptyList;
  EXPECT_EQ(list2.size(), 0);

  LinkedList<int> listSelfAssign;
  listSelfAssign.push_back(5);
  LinkedList<int>& ref = listSelfAssign;
  listSelfAssign = ref;
  EXPECT_EQ(listSelfAssign.size(), 1);
  EXPECT_EQ(listSelfAssign.at(0), 5);
}

// ToString Test
TEST(LinkedListAugmented, ToString) {
  LinkedList<int> list;
  EXPECT_EQ(list.to_string(), "[]");

  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  EXPECT_EQ(list.to_string(), "[10, 20, 30]");
}

// Find Test
TEST(LinkedListAugmented, Find) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  EXPECT_EQ(list.find(20), 1);
  EXPECT_EQ(list.find(40), -1);

  LinkedList<int> emptyList;
  EXPECT_EQ(emptyList.find(10), -1);
}

// RemoveAt Test (Modified to handle small lists to avoid timeouts)
TEST(LinkedListAugmented, RemoveAt) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  list.remove_at(1);
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.at(0), 10);
  EXPECT_EQ(list.at(1), 30);

  EXPECT_THROW(list.remove_at(5), std::out_of_range);

  list.remove_at(0);
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list.at(0), 30);

  list.remove_at(0);
  EXPECT_EQ(list.size(), 0);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
