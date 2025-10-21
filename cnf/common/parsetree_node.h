#ifndef INT_PARSETREE_NODE_H
#define INT_PARSETREE_NODE_H

#include <iostream>

struct NodeCNF {
    int data;  // positive integer literalCNF or negative operator
    NodeCNF* left;
    NodeCNF* right;

    NodeCNF(int val, NodeCNF* l = nullptr, NodeCNF* r = nullptr)
        : data(val), left(l), right(r) {}
};

// Copy a node recursively
inline NodeCNF* copyNodeCNF(const NodeCNF* src) {
    if (!src) return nullptr;
    NodeCNF* out = new NodeCNF(src->data);
    out->left = copyNodeCNF(src->left);
    out->right = copyNodeCNF(src->right);
    return out;
}

// NodeCNF constructors for operators and literals
inline NodeCNF* literalCNF(int value) {
    return new NodeCNF(value);
}  // value > 0
inline NodeCNF* negNodeCNF(NodeCNF* node) {
    return new NodeCNF(-4, nullptr, node);
}
inline NodeCNF* conjNodeCNF(NodeCNF* leftNode, NodeCNF* rightNode) {
    return new NodeCNF(-3, leftNode, rightNode);
}
inline NodeCNF* disjNodeCNF(NodeCNF* leftNode, NodeCNF* rightNode) {
    return new NodeCNF(-2, leftNode, rightNode);
}
inline NodeCNF* implNodeCNF(NodeCNF* leftNode, NodeCNF* rightNode) {
    return new NodeCNF(-1, leftNode, rightNode);
}

// Check if data is an operator
inline bool isOperatorCNF(int c) { return c <= -1 && c >= -4; }

// Check if node is a literalCNF
inline bool is_literal(const NodeCNF* node) {
    if (!node) return false;
    if (node->data > 0) return true;  // positive integer literalCNF
    if (node->data == -4) {           // negation
        return node->right && node->right->data > 0;
    }
    return false;  // other operators
}

// Operator precedence
inline int getPrecedenceCNF(int op) {
    switch (op) {
        case -1:
            return 1;  // implication
        case -2:
            return 2;  // OR
        case -3:
            return 3;  // AND
        case -4:
            return 4;  // negation
        default:
            return 0;  // literals
    }
}

// Recursively destroy parse tree
inline void destroyParseTreeCNF(NodeCNF* root) {
    if (!root) return;
    destroyParseTreeCNF(root->left);
    destroyParseTreeCNF(root->right);
    delete root;
}

#endif  // INT_PARSETREE_NODE_H