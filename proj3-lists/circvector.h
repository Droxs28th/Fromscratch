#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

template <typename T>
class CircVector {
 private:
  T *data;
  size_t vec_size;
  size_t capacity;
  size_t front_idx;

  // Private helper functions for resizing and wrapping index
  void resize() {
    size_t newCapacity = capacity * 2;
    T *newData = new T[newCapacity];
    for (size_t i = 0; i < vec_size; i++) {
      newData[i] = data[(front_idx + i) % capacity];
    }
    delete[] data;
    data = newData;
    front_idx = 0;
    capacity = newCapacity;
  }

  size_t wrapping(size_t index) const {
    return (front_idx + index) % capacity;
  }

 public:
  /**
   * Default constructor. Creates an empty `CircVector` with capacity 10.
   */
  CircVector() {
    capacity = 10;
    data = new T[capacity];
    vec_size = 0;
    front_idx = 0;
  }

  /**
   * Creates an empty `CircVector` with given capacity. Capacity must exceed 0.
   */
  CircVector(size_t cap) {
    if (cap <= 0) {
      throw std::invalid_argument("Capacity must exceed 0");
    }
    capacity = cap;
    data = new T[capacity];
    vec_size = 0;
    front_idx = 0;
  }

  /**
   * Returns whether the `CircVector` is empty (i.e. whether its
   * size is 0).
   */
  bool empty() const {
    return vec_size == 0;
  }

  /**
   * Returns the number of elements in the `CircVector`.
   */
  size_t size() const {
    return vec_size;
  }

  /**
   * Adds the given `T` to the front of the `CircVector`.
   */
  void push_front(T elem) {
    if (vec_size == capacity) {
      resize();
    }
    front_idx = (front_idx + capacity - 1) % capacity;
    data[front_idx] = elem;
    ++vec_size;
  }

  /**
   * Adds the given `T` to the back of the `CircVector`.
   */
  void push_back(T elem) {
    if (vec_size == capacity) {
      resize();
    }
    data[(front_idx + vec_size) % capacity] = elem;
    ++vec_size;
  }

  /**
   * Removes the element at the front of the `CircVector`.
   *
   * If the `CircVector` is empty, throws a `runtime_error`.
   */
  T pop_front() {
    if (vec_size == 0) {
      throw std::runtime_error("Cannot pop from an empty vector");
    }
    T frontElem = data[front_idx];
    front_idx = (front_idx + 1) % capacity;
    --vec_size;
    return frontElem;
  }

  /**
   * Removes the element at the back of the `CircVector`.
   *
   * If the `CircVector` is empty, throws a `runtime_error`.
   */
  T pop_back() {
    if (vec_size == 0) {
      throw std::runtime_error("Cannot pop from an empty vector");
    }
    size_t rearIdx = (front_idx + vec_size - 1) % capacity;
    T backElem = data[rearIdx];
    --vec_size;
    return backElem;
  }

  /**
   * Removes all elements from the `CircVector`.
   */
  void clear() {
    vec_size = 0;
    front_idx = 0;
  }

  /**
   * Destructor. Clears all allocated memory.
   */
  ~CircVector() {
    delete[] data;
  }

  /**
   * Returns the element at the given index in the `CircVector`.
   *
   * If the index is invalid, throws `out_of_range`.
   */
  T &at(size_t index) const {
    if (index >= vec_size) {
      throw std::out_of_range("Index out of range");
    }
    return data[(front_idx + index) % capacity];
  }

  /**
   * Copy constructor. Creates a deep copy of the given `CircVector`.
   *
   * Must run in O(N) time.
   */
  CircVector(const CircVector &other) {
    vec_size = other.vec_size;
    capacity = other.capacity;
    front_idx = other.front_idx;
    data = new T[capacity];
    for (size_t i = 0; i < vec_size; ++i) {
      data[i] = other.data[(front_idx + i) % capacity];
    }
  }

  /**
   * Assignment operator. Sets the current `CircVector` to a deep copy of the
   * given `CircVector`.
   *
   * Must run in O(N) time.
   */
  CircVector &operator=(const CircVector &other) {
    if (this != &other) {
      delete[] data;
      vec_size = other.vec_size;
      capacity = other.capacity;
      front_idx = other.front_idx;
      data = new T[capacity];
      for (size_t i = 0; i < vec_size; ++i) {
        data[i] = other.data[(front_idx + i) % capacity];
      }
    }
    return *this;
  }

  /**
   * Converts the `CircVector` to a string. Formatted like `[0, 1, 2, 3, 4]`
   * (without the backticks -- hover the function name to see). Runs in O(N)
   * time.
   */
  string to_string() const {
    if (vec_size == 0) {
      return "[]";
    }
    stringstream ss;
    ss << "[";
    for (size_t i = 0; i < vec_size; ++i) {
      ss << data[(front_idx + i) % capacity];
      if (i < vec_size - 1) {
        ss << ", ";
      }
    }
    ss << "]";
    return ss.str();
  }

  /**
   * Searches the `CircVector` for the first matching element, and returns its
   * index in the `CircVector`. If no match is found, returns "-1".
   */
  size_t find(const T &target) {
    for (size_t i = 0; i < vec_size; ++i) {
      if (data[(front_idx + i) % capacity] == target) {
        return i;
      }
    }
    return -1;
  }

  /**
   * Remove the element at the specified index in this list.
   *
   * If the index is invalid, throws `out_of_range`.
   */
  void remove_at(size_t index) {
    if (index >= vec_size) {
      throw std::out_of_range("Index out of range");
    }
    for (size_t i = index; i < vec_size - 1; ++i) {
      data[(front_idx + i) % capacity] = data[(front_idx + i + 1) % capacity];
    }
    --vec_size;
  }

  /**
   * Inserts the given `T` as a new element in the `CircVector` after
   * the given index. If the index is invalid, throws `out_of_range`.
   */
  void insert_after(size_t index, T elem) {
    if (index >= vec_size) {
      throw std::out_of_range("Index is out of range");
    }
    if (vec_size == capacity) {
      throw std::overflow_error("Cannot insert because the vector is full");
    }
    for (size_t i = vec_size; i > index + 1; --i) {
      data[(front_idx + i) % capacity] = data[(front_idx + i - 1) % capacity];
    }
    data[(front_idx + index + 1) % capacity] = elem;
    ++vec_size;
  }

  /**
   * Remove every other element (alternating) from the
   * `CircVector`, starting at index 1. Must run in O(N). May not reallocate.
   *
   * For example, if a list was `[0, 1, 2, 3, 4]`, removing every other element
   * would change the list to `[0, 2, 4]`.
   */
  void remove_every_other() {
    if (vec_size <= 1) return;
    size_t idx = 1;
    while (idx < vec_size) {
      for (size_t i = idx; i < vec_size - 1; ++i) {
        data[(front_idx + i) % capacity] = data[(front_idx + i + 1) % capacity];
      }
      --vec_size;
    }
  }

  /**
   * Returns a pointer to the underlying memory managed by the `CircVec`.
   * For autograder testing purposes only.
   */
  T *get_data() const {
    return data;
  }

  /**
   * Returns the capacity of the underlying memory managed by the `CircVec`. For
   * autograder testing purposes only.
   */
  size_t get_capacity() const {
    return capacity;
  }
};