# Satellite Network Router (Huffman Topology Builder)

Overview
This project is a C-based application designed to simulate, build, and optimize a hierarchical satellite network topology. It utilizes a custom-built Min-Heap and Binary Tree data structures to dynamically link nodes based on their communication frequency, closely following the logic of the Huffman Coding algorithm. 

Nodes with higher reporting frequencies are placed closer to the root (Earth), ensuring faster communication, while lower-frequency nodes are pushed further down the hierarchy.

Core Features & Algorithms
* Min-Heap Implementation: A fully custom Min-Heap used as a priority queue to efficiently extract the two satellites with the lowest reporting frequencies.
* Level-Order Traversal (BFS): Implemented using a custom Queue structure to display the resulting network layer by layer.
* Path Encoding & Decoding (DFS): * Parses binary sequences (`0` for left, `1` for right) to determine the exact satellite being communicated with.
  * Generates binary routing paths by traversing the tree to find specific target nodes.
* Lowest Common Ancestor (LCA): A recursive search algorithm that identifies the lowest shared parent node for a given set of malfunctioning satellites.
* Memory Management: Strictly ensures no memory leaks by recursively destroying trees, queues, and heaps (verified via Valgrind).

Technologies
* Language: C
* Concepts: Data Structures (Trees, Heaps, Queues), Algorithms (BFS, DFS, Huffman), Pointers, Dynamic Memory Allocation.

How to Build and Run

#Compilation
A `Makefile` is provided for easy compilation.
```bash
make build
./tema2 <task-flag> <input-file> <output-file>