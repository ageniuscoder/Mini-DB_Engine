# B-Tree Indexing Library

This is a C++ template library that provides a B-tree data structure designed for disk-based indexing in a mini-database. It efficiently stores key-offset pairs, allowing for fast searches, insertions, and deletions of records stored on a separate data file.

The B-tree is implemented as a template, making it flexible enough to work with various key types, such as `int`, `long`, `string`, or custom types.

## Features

- **Generic Implementation**: Uses C++ templates for a flexible key type.
- **On-Disk Serialization**: Supports saving and loading the B-tree index from a binary file.
- **Core Operations**: Includes efficient implementations of insertion, searching, and deletion.
- **Memory Management**: Handles node allocation and deallocation to prevent memory leaks.

## Project Structure

The project follows a standard modular file structure to keep the code organized and maintainable.

b-trees/
├── b_tree.h
├── b_tree_node.h
├── key_container.h



- `key_container.h`: A helper class that encapsulates a key and a file offset.
- `b_tree_node.h`: The core class representing a single node in the B-tree. It contains all the low-level logic for node management.
- `b_tree.h`: The main B-tree class that provides the public API for all tree operations. All implementation is in this header file to support C++ templates.

## Getting Started

To use this B-tree library in your `main.cpp` file, you need to include the primary header file and link to it during compilation.

### 1. Include the Header

In your `main.cpp` file, you only need to include the top-level `b_tree.h` header.

```cpp
// main.cpp
#include "b_tree.h"
#include <iostream>

// The rest of your main application code
// ...
2. Instantiate the B-Tree
The BTree class is a template that requires two type arguments and an optional integer argument for the minimum degree (t).

tree_type: The type of the key-value pair, which should be key_container<T>.

inner_type: The raw key type (e.g., int, long).

t: The minimum degree of the B-tree (defaults to 3).

Here is how you would instantiate a B-tree for integer keys:

C++

// For integer keys (e.g., record IDs)
BTree<key_container<int>, int, 3> my_index;
3. Basic Usage Example
You can now use the public methods of the BTree object to perform indexing operations.

C++

#include <iostream>
#include "b_tree.h"

int main() {
    // Instantiate a B-tree for integer keys
    BTree<key_container<int>, int, 3> my_index;

    // --- Insertion ---
    // Create key-offset pairs and insert them into the index
    my_index.insert(key_container<int>(100, 1000));
    my_index.insert(key_container<int>(50, 500));
    my_index.insert(key_container<int>(250, 2500));

    std::cout << "B-Tree contents after insertion:\n";
    my_index.levelOrderTraversal();

    // --- Searching ---
    // The search function returns a key_container object.
    // The main_key field is checked to see if the key was found.
    key_container<int> search_result = my_index.search(my_index.root, 50);

    if (search_result.main_key != -1) {
        std::cout << "\nFound key " << search_result.main_key
                  << " at offset " << search_result.key_offset << std::endl;
    } else {
        std::cout << "\nKey not found." << std::endl;
    }

    // --- Deletion ---
    std::cout << "\nRemoving key 100...\n";
    my_index.remove(100);
    my_index.levelOrderTraversal();
    
    // --- Serialization ---
    std::cout << "\nSaving index to disk...\n";
    my_index.disk_serialize("my_index_file.bin");
    
    // --- Loading from disk ---
    std::cout << "\nLoading index from disk...\n";
    BTree<key_container<int>, int, 3> loaded_index;
    loaded_index.load("my_index_file.bin");
    loaded_index.levelOrderTraversal();

    return 0;
}
4. Compilation
To compile your application, you must tell the compiler to look for the header files in the b-trees/ directory.

Bash

g++ -std=c++17 main.cpp -I b-trees/ -o my_database_app
Replace my_database_app with your desired executable name.