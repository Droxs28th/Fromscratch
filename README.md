# From Scratch

Four fundamental data structures implemented from the ground up in C++, 
with full test coverage — no STL containers used internally.

## Structures
- **LinkedList** — singly linked list with front/back operations, 
  O(1) push, deep copy semantics
- **CircVector** — circular buffer with dynamic resizing and wraparound 
  indexing, avoiding the shifting cost of a plain array-based vector
- **BSTMap** — binary search tree map supporting insert, erase (handling 
  leaf, one-child, and two-child deletion cases via in-order successor), 
  and iterative in-order traversal without recursion or an external stack
- **HashMap** — separate-chaining hash table with dynamic resizing based 
  on load factor, and unordered bucket traversal

## Design notes
- All four structures are templated (`template <typename KeyT, typename ValT>` 
  or similar) and support deep-copy construction and assignment
- BSTMap and HashMap both implement a stateful `begin()`/`next()` 
  iteration protocol instead of C++ iterators, for simplicity
- Every structure handles edge cases explicitly: empty-container operations 
  throw, self-assignment is safe, and copies are true deep copies (no 
  shared internal pointers)

## Testing
GoogleTest/GoogleMock suite covering:
- Core operations (insert, erase, lookup, iteration)
- Edge cases (empty containers, single elements, self-assignment)
- Collision handling (HashMap) and rebalancing scenarios (BSTMap deletion)
- Randomized stress tests

## Tech
C++, GoogleTest, GoogleMock, CMake/Make
