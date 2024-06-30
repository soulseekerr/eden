#pragma once

namespace edentree {

enum Color { RED, BLACK };

struct Node {
    int data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    Node(int data) {
        this->data = data;
        left = right = parent = nullptr;
        this->color = RED;
    }
};

class RedBlackTree {
private:
    Node* root;

public:
    RedBlackTree() : root(nullptr) {}

    // Function to insert a new node with given data
    void insert(const int& data);
    void inorder();
    void inorderHelper(Node* node);

protected:
    void rotateLeft(Node* root, Node* x);
    void rotateRight(Node* node1, Node* node2);
    void fixInsertRBTree(Node* node1, Node* node2);
};


} // namespace edentree