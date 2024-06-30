
#include "redblacktree.h"
#include <iostream>

using namespace edentree;

void RedBlackTree::insert(const int& data) {
    Node* p = new Node(data);

    // Do a normal BST insert
    if (root == nullptr) {
        root = p;

        // Fix Red-Black Tree violations
        fixInsertRBTree(root, p);
        return;
    }

    Node* tmp = root;
    Node* parent = nullptr;

    while (tmp != nullptr) {
        parent = tmp;
        if (p->data < tmp->data)
            tmp = tmp->left;
        else
            tmp = tmp->right;
    }

    p->parent = parent;
    if (p->data < parent->data)
        parent->left = p;
    else
        parent->right = p;
    
    // Fix Red-Black Tree violations
    fixInsertRBTree(root, p);
}

void RedBlackTree::inorder() {
    // inorder traversal
    inorderHelper(root);
}



void RedBlackTree::inorderHelper(Node* root) {
    if (root == nullptr) 
        return;
    
    inorderHelper(root->left);
    std::cout << root->data << " ";
    inorderHelper(root->right);
}

// Rotate subtree rooted with node x to the left
void RedBlackTree::rotateLeft(Node* root, Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (x->right != nullptr)
        x->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->left)
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Rotate subtree rooted with node x to the right
void RedBlackTree::rotateRight(Node* root, Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (x->left != nullptr)
        x->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->left)
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Fix violations caused by insertions
void RedBlackTree::fixInsertRBTree(Node* root, Node* p) {
    Node* parent_p = nullptr;
    Node* grand_parent_p = nullptr;

    while ((p != root) && (p->color != BLACK) &&
           (p->parent->color == RED)) {

        parent_p = p->parent;
        grand_parent_p = p->parent->parent;

        // Case A: Parent of pt is left child of Grand-parent of pt
        if (parent_p == grand_parent_p->left) {

            Node *uncle_p = grand_parent_p->right;

            // Case 1: The uncle of pt is also red
            if (uncle_p != nullptr && uncle_p->color == RED) {
                grand_parent_p->color = RED;
                parent_p->color = BLACK;
                uncle_p->color = BLACK;
                p = grand_parent_p;

            } else {
                // Case 2: pt is right child of its parent
                if (p == parent_p->right) {
                    rotateLeft(root, parent_p);
                    p = parent_p;
                    parent_p = p->parent;
                }

                // Case 3: pt is left child of its parent
                rotateRight(root, grand_parent_p);
                std::swap(parent_p->color, grand_parent_p->color);
                p = parent_p;
            }
        }
        // Case B: Parent of pt is right child of Grand-parent of pt
        else {
            Node *uncle_p = grand_parent_p->left;

            // Case 1: The uncle of pt is also red
            if ((uncle_p != nullptr) && (uncle_p->color == RED)) {
                grand_parent_p->color = RED;
                parent_p->color = BLACK;
                uncle_p->color = BLACK;
                p = grand_parent_p;

            } else {
                // Case 2: pt is left child of its parent
                if (p == parent_p->left) {
                    rotateRight(root, parent_p);
                    p = parent_p;
                    parent_p = p->parent;
                }

                // Case 3: pt is right child of its parent
                rotateLeft(root, grand_parent_p);
                std::swap(parent_p->color, grand_parent_p->color);
                p = parent_p;
            }
        }
    }

    root->color = BLACK;
}
