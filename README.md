# Data Structures Programs

This repository contains C++ programs for circular linked lists and AVL trees.

## Problem Statement 1: Sports Rotation System

Create a Sports Rotation System in which students can be added dynamically. The program must display all students and allow the sports captain role to move to the next student whenever the rotation option is selected. Use a circular linked list so that rotation continues from the first student after reaching the last student.

## Problem Statement 2: Delete a Node with Two Children from an AVL Tree

Implement a C++ program to delete a node with two children from an AVL tree. Replace the node using either its inorder successor or inorder predecessor, then rebalance the tree to maintain the AVL property.

## Problem Statement 3: AVL Tree with Insertion, Deletion, and Validation

Implement an AVL tree in C++ that supports insertion and deletion while maintaining strict balance. When deleting a node with two children, let the user choose whether to replace it with its inorder successor or inorder predecessor.

The program must:

- Log structural rotations triggered during rebalancing: Left, Right, Left-Right (LR), and Right-Left (RL).
- Verify the tree before and after deletion.
- Confirm that inorder traversal is strictly sorted.
- Validate that node heights and balance factors are mathematically correct.