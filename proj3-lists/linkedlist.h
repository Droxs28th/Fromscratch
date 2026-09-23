#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

template <typename T>
class LinkedList {
 private:
  class Node {
   public:
    T data;
    Node *next;

    Node(T data) {
      this->data = data;
      this->next = nullptr;
    }

    Node(T data, Node *next) {
      this->data = data;
      this->next = next;
    }
  };

  size_t list_size;
  Node *list_front;

 public:
  /**
   * Default constructor. Creates an empty `LinkedList`.
   */
  LinkedList() {
    this->list_front = nullptr;
    this->list_size = 0;
  }

  /**
   * Returns whether the `LinkedList` is empty (i.e. whether its
   * size is 0).
   */
  bool empty() const {
    return list_size == 0;
  }

  /**
   * Returns the number of elements in the `LinkedList`.
   */
  size_t size() const {
    return list_size;
  }

  /**
   * Adds the given `T` to the front of the `LinkedList`.
   */
  void push_front(T data) {
    Node *newNode = new Node(data);
    newNode->next = list_front;
    list_front = newNode;
    list_size++;
  }

  /**
   * Adds the given `T` to the back of the `LinkedList`.
   */
  void push_back(T data) {
    Node *newNode = new Node(data);
    if (list_front == nullptr) {
      list_front = newNode;
    } else {
      Node *currentNode = list_front;
      while (currentNode->next != nullptr) {
        currentNode = currentNode->next;
      }
      currentNode->next = newNode;
    }
    list_size++;
  }

  /**
   * Removes the element at the front of the `LinkedList`.
   *
   * If the `LinkedList` is empty, throws a `runtime_error`.
   */
  T pop_front() {
    if (list_front == nullptr) {
      throw std::runtime_error("Cannot pop from an empty list");
    }
    Node *temp = list_front;
    T data = temp->data;
    list_front = list_front->next;
    delete temp;
    list_size--;
    return data;
  }

  /**
   * Removes the element at the back of the `LinkedList`.
   *
   * If the `LinkedList` is empty, throws a `runtime_error`.
   */
  T pop_back() {
    if (list_front == nullptr) {
      throw std::runtime_error("Cannot pop from an empty list");
    }
    if (list_front->next == nullptr) {
      T data = list_front->data;
      delete list_front;
      list_front = nullptr;
      list_size--;
      return data;
    }
    Node *currentNode = list_front;
    while (currentNode->next->next != nullptr) {
      currentNode = currentNode->next;
    }
    T data = currentNode->next->data;
    delete currentNode->next;
    currentNode->next = nullptr;
    list_size--;
    return data;
  }

  /**
   * Empties the `LinkedList`, releasing all allocated memory, and resetting
   * member variables appropriately.
   */
  void clear() {
    Node *current = list_front;
    while (current != nullptr) {
      Node *nextNode = current->next;
      delete current;
      current = nextNode;
    }
    list_front = nullptr;
    list_size = 0;
  }

  /**
   * Destructor. Clears all allocated memory.
   */
  ~LinkedList() {
    clear();
  }

  /**
   * Returns the element at the given index in the `LinkedList`.
   *
   * If the index is invalid, throws `out_of_range`.
   */
  T &at(size_t index) const {
    if (index >= list_size) {
      throw std::out_of_range("Index out of range");
    }
    Node *current = list_front;
    for (size_t i = 0; i < index; ++i) {
      current = current->next;
    }
    return current->data;
  }

  /**
   * Copy constructor. Creates a deep copy of the given `LinkedList`.
   *
   * Must run in O(N) time.
   */
  LinkedList(const LinkedList &other) {
    this->list_front = nullptr;
    this->list_size = 0;
    if (other.list_front == nullptr) {
      return;
    }
    Node *current = other.list_front;
    while (current != nullptr) {
      this->push_back(current->data);
      current = current->next;
    }
  }

  /**
   * Assignment operator. Sets the current `LinkedList` to a deep copy of the
   * given `LinkedList`.
   *
   * Must run in O(N) time.
   */
  LinkedList &operator=(const LinkedList &other) {
    if (this == &other) {
      return *this;
    }
    this->clear();
    if (other.list_front == nullptr) {
      return *this;
    }
    Node *current = other.list_front;
    while (current != nullptr) {
      this->push_back(current->data);
      current = current->next;
    }
    return *this;
  }

  /**
   * Converts the `LinkedList` to a string. Formatted like `[0, 1, 2, 3, 4]`
   * (without the backticks -- hover the function name to see). Runs in O(N)
   * time.
   */
  string to_string() const {
    if (list_front == nullptr) {
      return "[]";
    }
    string result = "[";
    Node *current = list_front;
    while (current != nullptr) {
      result += std::to_string(current->data);
      if (current->next != nullptr) {
        result += ", ";
      }
      current = current->next;
    }
    result += "]";
    return result;
  }

  /**
   * Searches the `LinkedList` for the first matching element, and returns its
   * index. If no match is found, returns "-1".
   */
  size_t find(const T &data) {
    Node *current = list_front;
    size_t index = 0;
    while (current != nullptr) {
      if (current->data == data) {
        return index;
      }
      current = current->next;
      index++;
    }
    return -1;
  }

  /**
   * Remove the element at the specified index in this list.
   *
   * If the index is invalid, throws `out_of_range`.
   */
  void remove_at(size_t index) {
    if (index >= list_size) {
      throw std::out_of_range("Index is out of range");
    }
    if (index == 0) {
      pop_front();
      return;
    }
    Node *current = list_front;
    for (size_t i = 0; i < index - 1; ++i) {
      current = current->next;
    }
    Node *temp = current->next;
    current->next = temp->next;
    delete temp;
    list_size--;
  }

  /**
   * Inserts the given `T` as a new element in the `LinkedList` after
   * the given index. If the index is invalid, throws `out_of_range`.
   */
  void insert_after(size_t index, T data) {
    if (index >= list_size) {
      throw std::out_of_range("Index is out of range");
    }
    Node *current = list_front;
    for (size_t i = 0; i < index; ++i) {
      current = current->next;
    }
    Node *newNode = new Node(data);
    newNode->next = current->next;
    current->next = newNode;
    list_size++;
  }

  /**
   * Remove every other element (alternating) from the
   * `LinkedList`, starting at index 1. Must run in O(N).
   *
   * For example, if a list was `[0, 1, 2, 3, 4]`, removing every other element
   * would change the list to `[0, 2, 4]`.
   */
  void remove_every_other() {
    if (list_front == nullptr || list_front->next == nullptr) {
      return;
    }

    Node *current = list_front;
    while (current != nullptr && current->next != nullptr) {
      Node *temp = current->next;
      current->next = temp->next;
      delete temp;
      current = current->next;
      list_size--;
    }
  }

  /**
   * Returns a pointer to the node at the front of the `LinkedList`. For
   * autograder testing purposes only.
   */
  void *front() const {
    return this->list_front;
  }
};