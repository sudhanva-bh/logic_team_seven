#ifndef INT_PARSETREE_NODE_H
#define INT_PARSETREE_NODE_H

#include <iostream>

struct Node {
    int data;      // positive integer literal or negative operator
    Node* left;
    Node* right;

    Node(int val, Node* l = nullptr, Node* r = nullptr)
        : data(val), left(l), right(r) {}
};

// Copy a node recursively
inline Node* copyNode(const Node* src) {
    if (!src) return nullptr;
    Node* out = new Node(src->data);
    out->left = copyNode(src->left);
    out->right = copyNode(src->right);
    return out;
}

// Node constructors for operators and literals
inline Node* literal(int value) { return new Node(value); }   // value > 0
inline Node* negationNode(Node* node) { return new Node(-4, nullptr, node); }
inline Node* conjunctionNode(Node* leftNode, Node* rightNode) { return new Node(-3, leftNode, rightNode); }
inline Node* disjunctionNode(Node* leftNode, Node* rightNode) { return new Node(-2, leftNode, rightNode); }
inline Node* implication(Node* leftNode, Node* rightNode) { return new Node(-1, leftNode, rightNode); }

// Check if data is an operator
inline bool isOperator(int c) {
    return c <= -1 && c >= -4;
}

// Check if node is a literal
inline bool is_literal(const Node* node) {
    if (!node) return false;
    if (node->data > 0) return true;  // positive integer literal
    if (node->data == -4) {           // negationNode
        return node->right && node->right->data > 0;
    }
    return false;                     // other operators
}

// Operator precedence
inline int getPrecedence(int op) {
    switch (op) {
        case -1: return 1; // implication
        case -2: return 2; // OR
        case -3: return 3; // AND
        case -4: return 4; // negationNode
        default: return 0; // literals
    }
}

// Recursively destroy parse tree
inline void destroyParseTree(Node* root) {
    if (!root) return;
    destroyParseTree(root->left);
    destroyParseTree(root->right);
    delete root;
}

#endif  // INT_PARSETREE_NODE_H
