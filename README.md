# Segmentation Resort 🏨

A C++ object-oriented data structures project developed for the Data Structures course at the Technion – Israel Institute of Technology.

## Project Overview
Segmentation Resort is a hotel and dining management system designed to track guests, assigned rooms, dining tables, and restaurant seating arrangements. The system efficiently handles guest check-ins/check-outs, dining hall occupancy, table merging operations, meal rotation tracking, and circular room-cleaning duties while adhering strictly to logarithmic and linear time complexity bounds.

## Technical Highlights
* **Data Structures:** Custom generic AVL Trees supporting self-balancing rotations and bottom-up construction; doubly-linked circular lists embedded within room nodes; composite linking nodes.
* **Key Algorithms:** In-order array extraction and linear-time merging of sorted sub-trees for table amalgamations; tree successor/predecessor navigation for ordered circular links; recursive post-order tree cleanup.
* **Memory Management:** Implemented full custom dynamic memory management without using C++ STL containers or algorithms.

## File Structure
* `AVLTree.h` — Custom generic AVL tree template implementation.
* `TreeNode.h` — Generic tree node class.
* `Nodes.h` — Struct definitions for `GuestNode`, `RoomNode`, `TableNode`, and `GuestSeatNode`.
* `SegmentationResort26b1.h` / `.cpp` — Main system controller and algorithm implementations.