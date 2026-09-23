#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

template <typename KeyT, typename ValT>
class HashMap {
 private:
  struct ChainNode {
    const KeyT key;
    ValT value;
    ChainNode* next;

    ChainNode(KeyT key, ValT value) : key(key), value(value), next(nullptr) {
    }

    ChainNode(KeyT key, ValT value, ChainNode* next)
        : key(key), value(value), next(next) {
    }
  };

  ChainNode** data;
  size_t sz;
  size_t capacity;

  // Utility members for begin/next
  ChainNode* curr;
  size_t curr_idx;

  // Helper Functions
  void resize(size_t new_capacity) {
    ChainNode** new_data = new ChainNode*[new_capacity];
    for (size_t i = 0; i < new_capacity; ++i) {
      new_data[i] = nullptr;
    }

    for (size_t i = 0; i < capacity; ++i) {
      ChainNode* node = data[i];
      while (node != nullptr) {
        ChainNode* next_node = node->next;

        size_t new_index = hash<KeyT>{}(node->key) % new_capacity;
        node->next = new_data[new_index];
        new_data[new_index] = node;

        node = next_node;
      }
    }

    delete[] data;
    data = new_data;
    capacity = new_capacity;
  }

 public:
  /**
   * Creates an empty `HashMap` with 10 buckets.
   */
  HashMap() {
    // TODO_STUDENT
    this->capacity = 10;
    this->sz = 0;
    this->data = new ChainNode*[capacity];
    for (size_t i = 0; i < capacity; i++) {
      data[i] = nullptr;
    }
    curr = nullptr;
    curr_idx = 0;
  }

  /**
   * Creates an empty `HashMap` with `capacity` buckets.
   */
  HashMap(size_t capacity) {
    // TODO_STUDENT
    this->capacity = capacity;
    this->sz = 0;
    this->data = new ChainNode*[capacity];
    for (int i = 0; i < capacity; i++) {
      data[i] = nullptr;
    }
    curr = nullptr;
    curr_idx = 0;
  }

  /**
   * Checks if the `HashMap` is empty. Runs in O(1).
   */
  bool empty() const {
    // TODO_STUDENT
    return sz == 0;
  }

  /**
   * Returns the number of mappings in the `HashMap`. Runs in O(1).
   */
  size_t size() const {
    // TODO_STUDENT
    return sz;
  }

  /**
   * Adds the mapping `{key -> value}` to the `HashMap`. If the key already
   * exists, does not update the mapping (like the C++ STL map).
   *
   * Uses the hash value of the key to determine the location in the
   * underlying hash table. Creates exactly one new node; resizes by doubling
   * when the load factor exceeds 1.5.
   *
   * On resize, doesn't create new nodes, but rearranges existing ones.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  void insert(KeyT key, ValT value) {
    size_t index = hash<KeyT>{}(key) % capacity;
    ChainNode* node = data[index];

    while (node != nullptr) {
      if (node->key == key) {
        return;
      }
      node = node->next;
    }

    ChainNode* newNode = new ChainNode(key, value, data[index]);
    data[index] = newNode;
    sz++;

    if ((double)sz / capacity > 1.5) {
      resize(capacity * 2);
    }
  }

  /**
   * Return a reference to the value stored for `key` in the map.
   *
   * If key is not present in the map, throw `out_of_range` exception.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  ValT& at(const KeyT& key) const {
    // TODO_STUDENT
    size_t index = hash<KeyT>{}(key) % capacity;
    ChainNode* node = data[index];

    while (node != nullptr) {
      if (node->key == key) {
        return node->value;
      }
      node = node->next;
    }

    throw out_of_range("Key not found in HashMap");
  }

  /**
   * Returns `true` if the key is present in the map, and false otherwise.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  bool contains(const KeyT& key) const {
    size_t index = hash<KeyT>{}(key) % capacity;
    ChainNode* node = data[index];

    while (node != nullptr) {
      if (node->key == key) {
        return true;
      }
      node = node->next;
    }

    return false;
  }

  /**
   * Empties the `HashMap`, freeing all nodes. The bucket array may be left
   * alone.
   *
   * Runs in O(N+B), where N is the number of mappings and B is the number of
   * buckets.
   */
  void clear() {
    for (size_t i = 0; i < capacity; ++i) {
      ChainNode* node = data[i];
      while (node != nullptr) {
        ChainNode* next_node = node->next;
        delete node;
        node = next_node;
      }
      data[i] = nullptr;
    }
    delete[] data;
    data = nullptr;
    sz = 0;
    capacity = 0;
  }

  /**
   * Destructor, cleans up the `HashMap`.
   *
   * Runs in O(N+B), where N is the number of mappings and B is the number of
   * buckets.
   */
  ~HashMap() {
    // TODO_STUDENT
    for (size_t i = 0; i < capacity; ++i) {
      ChainNode* node = data[i];
      while (node != nullptr) {
        ChainNode* temp = node;
        node = node->next;
        delete temp;
      }
    }
    delete[] data;
  }

  /**
   * Removes the mapping for the given key from the `HashMap`, and returns the
   * value.
   *
   * Throws `out_of_range` if the key is not present in the map. Creates no new
   * nodes, and does not update the key or value of any existing nodes.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  ValT erase(const KeyT& key) {
    // TODO_STUDENT
    size_t index = hash<KeyT>{}(key) % capacity;
    ChainNode* node = data[index];
    ChainNode* prev = nullptr;

    while (node != nullptr) {
      if (node->key == key) {
        if (prev == nullptr) {
          data[index] = node->next;
        } else {
          prev->next = node->next;
        }
        ValT value = node->value;
        delete node;
        sz--;
        return value;
      }
      prev = node;
      node = node->next;
    }

    throw out_of_range("Key not found in HashMap");
  }

  /**
   * Copy constructor.
   *
   * Copies the mappings from the provided `HashMap`.
   *
   * Runs in O(N+B), where N is the number of mappings in `other`, and B is the
   * number of buckets.
   */
  HashMap(const HashMap& other) {
    // TODO_STUDENT
    capacity = other.capacity;
    sz = other.sz;
    curr = nullptr;
    curr_idx = 0;

    data = new ChainNode*[capacity]();

    for (size_t i = 0; i < capacity; ++i) {
      ChainNode* node = other.data[i];
      ChainNode* prev = nullptr;

      while (node != nullptr) {
        ChainNode* new_node = new ChainNode(node->key, node->value);
        if (prev == nullptr) {
          data[i] = new_node;
        } else {
          prev->next = new_node;
        }
        prev = new_node;
        node = node->next;
      }
    }
  }

  /**
   * Assignment operator; `operator=`.
   *
   * Clears this table, and copies the mappings from the provided `HashMap`.
   *
   * Runs in O((N1+B1) + (N2+B2)), where N1 and B1 are the number of mappings
   * and buckets in `this`, and N2 and B2 are the number of mappings and buckets
   * in `other`.
   */
  HashMap& operator=(const HashMap& other) {
    // TODO_STUDENT
    if (this == &other) {
      return *this;
    }

    this->clear();

    capacity = other.capacity;
    sz = other.sz;

    data = new ChainNode*[capacity]();
    for (size_t i = 0; i < capacity; ++i) {
      ChainNode* node = other.data[i];
      ChainNode* prev = nullptr;

      while (node != nullptr) {
        ChainNode* new_node = new ChainNode(node->key, node->value);
        if (prev == nullptr) {
          data[i] = new_node;
        } else {
          prev->next = new_node;
        }
        prev = new_node;
        node = node->next;
      }
    }

    return *this;
  }

  // =====================

  /**
   *
   * Checks if the contents of `this` and `other` are equivalent.
   *
   * Two `HashMap` objects are equivalent if they contain the same
   * mappings from key to value. To be equivalent, the two `HashMap`
   * objects need not have the elements saved in the same order
   * inside of the buckets.
   *
   * Runs in worst-case O(B*L) time, where B is the maximum number
   * of buckets in either of the `HashMap` objects and L is the length
   * of the  largest chain on any of the buckets.
   *
   */
  bool operator==(const HashMap& other) const {
    // If sizes differ, they can't be equal
    if (sz != other.sz) {
      return false;
    }

    // Check if every key-value pair in this map exists in other
    for (size_t i = 0; i < capacity; ++i) {
      ChainNode* node = data[i];
      while (node != nullptr) {
        if (!other.contains(node->key)) {
          return false;
        }
        if (other.at(node->key) != node->value) {
          return false;
        }
        node = node->next;
      }
    }

    return true;
  }

  /**
   * Resets internal state for an iterative traversal.
   *
   * See `next` for usage details. Modifies nothing except for `curr` and
   * `curr_idx`.
   *
   * Runs in worst-case O(B), where B is the number of buckets.
   */
  void begin() {
    // TODO_STUDENT
    curr_idx = 0;
    curr = nullptr;
    while (curr_idx < capacity) {
      if (data[curr_idx] != nullptr) {
        curr = data[curr_idx];
        return;
      }
      curr_idx++;
    }
  }

  /**
   * Uses the internal state to return the "next" key and value
   * by reference, and advances the internal state. Returns `true` if the
   * reference parameters were set, and `false` otherwise.
   *
   * Example usage:
   *
   * ```c++
   * HashMap<string, int> hm;
   * hm.begin();
   * string key;
   * int value;
   * while (hm.next(key, val)) {
   *   cout << key << ": " << val << endl;
   * }
   * ```
   *
   * Does not visit the mappings in any defined order.
   *
   * Modifies nothing except for `curr` and `curr_idx`.
   *
   * Runs in worst-case O(B) where B is the number of buckets.
   */
  bool next(KeyT& key, ValT& value) {
    // TODO_STUDENT
    if (curr == nullptr) {
      return false;
    }
    key = curr->key;
    value = curr->value;
    curr = curr->next;
    if (curr == nullptr) {
      curr_idx++;
      while (curr_idx < capacity) {
        if (data[curr_idx] != nullptr) {
          curr = data[curr_idx];
          break; 
        }
        curr_idx++;
      }
    }
    return true;
  }

  // ===============================================

  /**
   * Returns a pointer to the underlying memory managed by the `HashMap`.
   * For autograder testing purposes only.
   */
  void* get_data() const {
    return this->data;
  }

  /**
   * Returns the capacity of the underlying memory managed by the `HashMap`. For
   * autograder testing purposes only.
   */
  size_t get_capacity() {
    return this->capacity;
  }
};
