# JumpList Data Structure - CO3105 C++ Programming Coursework

A C++ implementation of a JumpList - an efficient data structure that combines the benefits of linked lists with jump pointers for faster searching and traversal.

## Overview

The JumpList is a specialized linked list where certain nodes (called "jump nodes") maintain additional pointers that allow skipping over segments of the list. This design provides improved search performance compared to traditional linked lists while maintaining dynamic insertion and deletion capabilities.

## Features

- **Efficient Search**: O(√n) average search time using jump pointers
- **Dynamic Operations**: Support for insertion and deletion with automatic segment management
- **Sorted Storage**: Maintains elements in lexicographical order
- **Memory Efficient**: Balanced memory usage with configurable segment sizes
- **Visual Output**: Built-in pretty printing for debugging and visualization

## Data Structure Design

### Node Structure
Each node contains:
- `data_`: String value stored in the node
- `next_`: Pointer to the next node in sequence
- `jump_`: Pointer to the next jump node (null for non-jump nodes)
- `gap_`: Size of the segment starting from this node

### Jump Node Strategy
- Jump nodes are placed at regular intervals (configurable via `MAX_GAP_SIZE = 5`)
- Each jump node tracks the size of its segment
- When segments exceed the maximum size, they are automatically split
- When segments become too small after deletions, they are merged

## Core Operations

### Search Algorithm
1. **Fast Lane**: Traverse jump pointers until the target segment is identified
2. **Slow Lane**: Linear search within the identified segment

### Insertion
1. Locate the correct position using the search algorithm
2. Insert the new node maintaining sorted order
3. Update segment sizes and split if necessary:
   - When a segment exceeds `MAX_GAP_SIZE` after insertion, split into two equal segments
   - If the segment size is odd, the first segment gets the extra node
4. Recalculate jump pointers when segments are restructured

### Deletion
1. Locate and remove the target node
2. Handle different cases:
   - **Non-jump node**: Update gap size of the relevant jump node
   - **Jump node**: Merge adjacent segments, split if resulting segment > `MAX_GAP_SIZE`
   - **Head node**: Next node becomes new head and jump node
3. Properly release memory for removed nodes

## Class Interface

### JumpList Class
```cpp
class JumpList {
public:
    JumpList();                                    // Default constructor
    JumpList(int size, const string* arr = nullptr); // Parameterized constructor
    ~JumpList();                                   // Destructor
    
    int size() const;                              // Get number of elements
    bool find(const string& s) const;             // Search for element
    string get(int i) const;                      // Get element at index
    
    bool insert(const string& s);                 // Insert element
    bool erase(const string& s);                  // Remove element
    
    string print() const;                         // Basic string representation
    string prettyPrint() const;                   // Formatted visualization
};
```

**Note on Parameterized Constructor**: When `arr` is `nullptr`, the constructor creates a truncated version of a predefined example list based on the `size` parameter (1-node, 3-node, or 10-node versions).

### Node Class
```cpp
class Node {
    string data_;
    Node* next_;
    Node* jump_;
    int gap_;
public:
    Node(const string& data, Node* next, Node* jump, int gap);
    ~Node();
};
```

## Usage Examples

### Basic Operations
```cpp
#include "JumpList.h"

int main() {
    JumpList list;
    
    // Insert elements
    list.insert("apple");
    list.insert("banana");
    list.insert("cherry");
    
    // Search
    bool found = list.find("banana"); // returns true
    
    // Access by index
    string first = list.get(0); // returns "apple"
    
    // Remove element
    list.erase("banana");
    
    return 0;
}
```

### Visualization
```cpp
JumpList example(10); // Creates example list
cout << example.prettyPrint() << endl;
```

Output:
```
a --> b --> blah --> c --> cat --> d --> etc --> ooo... --> x --> zzz
a -------------------------------> d ---------------------> x
5                                  3                        2
```

## File Structure

- `JumpList.h` - Class declarations and interface
- `JumpList.cpp` - Implementation of all JumpList operations
- `JumpListTester.h` - Comprehensive test suite header
- `JumpListTester.cpp` - Test implementation with 20+ test cases
- `JumpListTesterMain.cpp` - Test runner main function
- `main.cpp` - Example usage and demonstration

## Building and Testing

### Compilation
```bash
g++ -std=c++11 -o main main.cpp JumpList.cpp
g++ -std=c++11 -o tester JumpListTesterMain.cpp JumpListTester.cpp JumpList.cpp
```

### Running Tests
```bash
# Compile the test suite
make

# Run specific tests
./JumpListTesterMain a b c d e f g h i j k l m n o p q r s t u v

# Run individual test
./JumpListTesterMain a
```

### Test Coverage
The test suite includes:
- **Basic Operations**: Constructor, size, find, get
- **Insertion**: Empty list, sorted insertion, segment splitting
- **Deletion**: Various positions, segment merging, edge cases
- **Visualization**: Print and pretty print functionality
- **Edge Cases**: Empty lists, single elements, boundary conditions

## Implementation Details

### Memory Management
- Proper cleanup in destructor to prevent memory leaks
- Careful pointer management during insertions and deletions
- Exception safety for dynamic operations

### Segment Management
- Automatic splitting when segments exceed `MAX_GAP_SIZE`
- Intelligent merging when segments become too small
- Balanced segment sizes for optimal performance

### Error Handling
- Bounds checking for array access
- Validation for insertion/deletion operations
- Graceful handling of edge cases

## Implementation Constraints and Guidelines

### Technology Restrictions
- **No STL containers**: Standard Template Library containers were not used
- **No smart pointers**: Use raw pointers with manual memory management
- **No external libraries**: Implementation uses only standard C++ features

### Special Implementation Notes
- This is a simplified educational data structure, intentionally different from production skip lists
- Some design choices are sub-optimal by design for learning purposes

## Complex Deletion Examples

### Sequential Deletion Scenario
When all non-jump nodes are deleted first:
```
a --> d --> x
a --> d --> x
1     1     1
```

Then deleting head node 'a':
```
d --> x
d --> x
1     1
```

### Segment Merging with Odd Size Split
Deleting jump node 'd' from running example creates a 7-node segment that splits into 4:3 (first segment gets extra node):
```
a --> b --> blah --> c --> cat --> etc --> ooo... --> x --> zzz
a -----------------------> cat ---------------------> x
4                          3                          2
```

### Additional Deletion Examples

Example 1: delete "blah" from the running example:
```
a --> b --> c --> cat --> d --> etc --> ooo... --> x --> zzz
a ----------------------> d ---------------------> x
4                         3                        2
```
(Not a jump node; the gap size for 'a' is updated)

Example 2: delete "x" from the running example:
```
a --> b --> blah --> c --> cat --> d --> etc --> ooo... --> zzz
a -------------------------------> d
5                                  4
```
(The final two segments are merged into one)

Example 3: delete "a" from the running example:
```
b --> blah --> c --> cat --> d --> etc --> ooo... --> x --> zzz
b -------------------------> d ---------------------> x
4                            3                        2
```
(b becomes the head and a jump node)

## Technical Notes

- Requires C++11 or later
- Uses STL string class for data storage
- Implements custom memory management for linked structure
- Thread-safe for read operations (not thread-safe for modifications)

## Educational Context

This project focuses on:
- Clean, readable code structure
- Comprehensive error handling
- Extensive test coverage
- Memory safety and efficiency
- Understanding of pointer manipulation and dynamic memory management

---
