#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

template <typename KeyT, typename ValT>
class BSTMap {
 private:
  struct BSTNode {
    const KeyT key;  // This member should never change after initialization!
    ValT value;
    BSTNode* parent;
    BSTNode* left;
    BSTNode* right;

    // This is fancy constructor syntax
    // Don't worry too much about it -- it's just necesssary for the const
    // member.
    BSTNode(KeyT key, ValT value, BSTNode* parent)
        : key(key),
          value(value),
          parent(parent),
          left(nullptr),
          right(nullptr) {
    }
  };

  BSTNode* root;
  size_t sz;

  // Utility pointer for begin and next.
  BSTNode* curr;

  // TODO_STUDENT: add private helper function definitions here
  void clean_helper(BSTNode* node) const {
    if (node) {
      clean_helper(node->left);
      clean_helper(node->right);
      delete node;
    }
  }

  void to_string_helper(BSTNode* node, ostringstream& string) const {
    if (node != nullptr) {
      to_string_helper(node->left, string);
      string << node->key << ": " << node->value << endl;
      to_string_helper(node->right, string);
    }
  }

  BSTNode* copy_helper(BSTNode* node, BSTNode* parent) {
    if (node == nullptr) {
      return nullptr;
    }
    BSTNode* newNode = new BSTNode(node->key, node->value, parent);
    newNode->left = copy_helper(node->left, newNode);
    newNode->right = copy_helper(node->right, newNode);
    return newNode;
  }

  BSTNode* findMin(BSTNode* node) const {
    while (node && node->left) {
      node = node->left;
    }
    return node;
  }

  bool equals_helper(BSTNode* node, const BSTMap& other) const {
    if (node == nullptr) {
      return true;
    }
    if (!equals_helper(node->left, other)) return false;
    try {
      if (other.at(node->key) != node->value) return false;
    } catch (out_of_range&) {
      return false;
    }
    return equals_helper(node->right, other);
  }

 public:
  /**
   * Creates an empty `BSTMap`. Runs in O(1).
   */
  BSTMap() {
    // TODO_STUDENT
    root = nullptr;
    sz = 0;
  }

  /**
   * Checks if the `BSTMap` is empty. Runs in O(1).
   */
  bool empty() const {
    // TODO_STUDENT
    return sz == 0;
  }

  /**
   * Returns the number of mappings in the `BSTMap`. Runs in O(1).
   */
  size_t size() const {
    // TODO_STUDENT
    return sz;
  }

  /**
   * Adds the mapping `{key -> value}` to the `BSTMap`. If the key already
   * exists, does not update the mapping (like the C++ STL map).
   *
   * Uses the key to determine the location in the underlying BST. Creates
   * exactly one new node.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  void insert(KeyT key, ValT value) {
    // TODO_STUDENT
    BSTNode* curr = root;
    BSTNode* parent = nullptr;
    while (curr != nullptr) {
      parent = curr;
      if (key < curr->key) {
        curr = curr->left;
      } else if (key > curr->key) {
        curr = curr->right;
      } else {
        return;
      }
    }

    BSTNode* newNode = new BSTNode(key, value, parent);
    if (parent == nullptr) {
      root = newNode;
    } else if (key < parent->key) {
      parent->left = newNode;
    } else {
      parent->right = newNode;
    }
    sz++;
  }

  /**
   * Return a reference to the value stored for `key` in the map.
   *
   * If key is not present in the map, throw `out_of_range` exception.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  ValT& at(const KeyT& key) const {
    // TODO_STUDENT
    BSTNode* curr = root;
    while (curr != nullptr) {
      if (key < curr->key) {
        curr = curr->left;
      } else if (key > curr->key) {
        curr = curr->right;
      } else {
        return curr->value;
      }
    }
    throw out_of_range("Key not found");
  }

  /**
   * Returns `true` if the key is present in the map, and false otherwise.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  bool contains(const KeyT& key) const {
    // TODO_STUDENT
    BSTNode* curr = root;
    while (curr != nullptr) {
      if (key < curr->key) {
        curr = curr->left;
      } else if (key > curr->key) {
        curr = curr->right;
      } else {
        return true;
      }
    }
    return false;
  }

  /**
   * Empties the `BSTMap`, freeing all memory it controls.
   *
   * Runs in O(N), where N is the number of mappings.
   */
  void clear() {
    // TODO_STUDENT
    clean_helper(root);
    root = nullptr;
    sz = 0;
  }

  /**
   * Destructor, cleans up the `BSTMap`.
   *
   * Runs in O(N), where N is the number of mappings.
   */
  ~BSTMap() {
    // TODO_STUDENT
    clear();
  }

  /**
   * Converts the `BSTMap` to a string representation, with the values
   * in-order by key.
   *
   * Example:
   *
   * ```c++
   * BSTMap<string, int> names;
   * names.insert("Gwen", 10);
   * names.insert("Jen", 5);
   * names.insert("Ben", 10);
   * names.insert("Sven", 7);
   * ```
   *
   * Calling `names.to_string()` would return the following **multi-line**
   * string:
   *
   * ```text
   * Ben: 10
   * Gwen: 10
   * Jen: 5
   * Sven: 7
   * ```
   *
   * Runs in O(N), where N is the number of mappings.
   */
  string to_string() const {
    // TODO_STUDENT
    ostringstream string;
    to_string_helper(root, string);
    return string.str();
  }

  /**
   * Copy constructor.
   *
   * Copies the mappings from the provided `BSTMap`.
   * The internal tree structure must be copied exactly.
   *
   * Runs in O(N), where N is the number of mappings in `other`.
   */
  BSTMap(const BSTMap& other) {
    // TODO_STUDENT
    root = copy_helper(other.root, nullptr);
    sz = other.sz;
  }

  /**
   * Assignment operator; `operator=`.
   *
   * Clears this tree, and copies the mappings from the provided `BSTMap`.
   * The internal tree structure must be copied exactly.
   *
   * Runs in O(N + O), where N is the number of mappings in `this`, and O is
   * the number of mappings in `other`.
   */
  BSTMap& operator=(const BSTMap& other) {
    // TODO_STUDENT
    if (this != &other) {
      clear();
      root = copy_helper(other.root, nullptr);
      sz = other.sz;
    }
    return *this;
  }

  // =========================================================================

  /**
   * Locates the smallest key in the BSTMap, and removes and returns its
   * mapping. If the `BSTMap` is empty, throws a `runtime_error`.
   *
   * Creates no new nodes, and does not update the key or value of any
   * existing nodes.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  pair<KeyT, ValT> remove_min() {
    // TODO_STUDENT
    if (!root) throw runtime_error("remove_min from empty tree");
    BSTNode* minNode = findMin(root);
    pair<KeyT, ValT> minPair(minNode->key, minNode->value);
    BSTNode* parent = minNode->parent;
    if (minNode->right) minNode->right->parent = parent;
    if (parent) {
      parent->left = minNode->right;
    } else {
      root = minNode->right;
    }
    delete minNode;
    sz--;
    return minPair;
  }

  /**
   * Checks if the contents of `this` and `other` are equivalent.
   *
   * Two `BSTMap` objects are equivalent if they contain the same
   * mappings from key to value. To be equivalent, the two `BSTMap`
   * objects need not have the same internal tree structure.
   *
   *
   * Two different approaches could be used here.
   *
   * Approach #1:
   * Runs in worst-case O(N) time, using worst-case O(N) heap space,
   * where N is the maximum number of nodes in either of the `BSTMap`
   * objects. Since you cannot include other C++ classes, the extra
   * space in this approach would have to be used with arrays or
   * with `string` objects.
   *
   * Approach #2:
   * Runs in worst-case O(N*H) time, using worst-case O(1) space on
   * the heap, where N is the maximum number of nodes in either of
   * the `BSTMap` objects and H is the largest height between the two
   * `BSTMap` objects.
   *
   */
  bool operator==(const BSTMap& other) const {
    // TODO_STUDENT
    if (sz != other.sz) return false;
    return equals_helper(root, other);
  }

  /**
   * Resets internal state for an iterative inorder traversal.
   *
   * See `next` for usage details. Modifies nothing except for `curr`.
   *
   * O(H), where H is the maximum height of the tree.
   */
  void begin() {
    // TODO_STUDENT
    curr = root;
    if (!curr) return;
    while (curr->left) {
      curr = curr->left;
    }
  }

  /**
   * Uses the internal state to return the next in-order key and value
   * by reference, and advances the internal state. Returns `true` if the
   * reference parameters were set, and `false` otherwise.
   *
   * Example usage:
   *
   * ```c++
   * BSTMap<string, int> bst;
   * bst.begin();
   * string key;
   * int value;
   * while (bst.next(key, val)) {
   *   cout << key << ": " << val << endl;
   * }
   * ```
   *
   * Modifies nothing except for `curr`.
   *
   * Runs in worst-case O(H) where H is the height of the tree.
   */
  bool next(KeyT& key, ValT& val) {
    // TODO_STUDENT
    if (!curr) return false;
    key = curr->key;
    val = curr->value;

    if (curr->right) {
      curr = curr->right;
      while (curr->left) curr = curr->left;
    } else {
      BSTNode* parent = curr->parent;
      while (parent && curr == parent->right) {
        curr = parent;
        parent = parent->parent;
      }
      curr = parent;
    }
    return true;
  }

  // =========================================================================

  /**
   * Removes the mapping for the given key from the `BSTMap`, and returns the
   * value.
   *
   * Throws `out_of_range` if the key is not present in the map. Creates no new
   * nodes, and does not update the key or value of any existing nodes.
   *
   * Runs in worst-case O(H) where H is the height of the tree.
   */
  ValT erase(const KeyT& key) {
    BSTNode* node = root;
    while (node && node->key != key) {
      if (key < node->key) {
        node = node->left;
      } else {
        node = node->right;
      }
    }

    if (!(node)) {
      throw out_of_range("Key not found");
    }

    ValT removedValue = node->value;

    if (!node->left || !node->right) {
      BSTNode* child;
      if (node->left) {
        child = node->left;
      } else {
        child = node->right;
      }
      if (child) {
        child->parent = node->parent;
      }

      if (!node->parent) {
        root = child;
      } else if (node == node->parent->left) {
        node->parent->left = child;
      } else {
        node->parent->right = child;
      }

      delete node;
    } else {
      BSTNode* successor = findMin(node->right);

      if (successor->parent != node) {
        if (successor->right) {
          successor->right->parent = successor->parent;
        }
        successor->parent->left = successor->right;
        successor->right = node->right;
        if (successor->right) {
          successor->right->parent = successor;
        }
      }

      successor->left = node->left;
      if (successor->left) {
        successor->left->parent = successor;
      }

      successor->parent = node->parent;
      if (!node->parent) {
        root = successor;
      } else if (node == node->parent->left) {
        node->parent->left = successor;
      } else {
        node->parent->right = successor;
      }
      delete node;
    }
    sz--;
    return removedValue;
  }

  // =========================================================================

  /**
   * Returns a pointer to the root node of the `BSTMap`. For autograder testing
   * purposes only.
   */
  void* getRoot() const {
    return this->root;
  }
};
