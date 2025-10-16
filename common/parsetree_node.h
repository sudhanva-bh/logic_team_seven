#ifndef PARSETREE_NODE_H
#define PARSETREE_NODE_H

#include <iostream>

struct Node {
    char data;
    Node* left;
    Node* right;
    
    Node(char val, Node* l = nullptr, Node* r = nullptr)
        : data(val), left(l), right(r) {}
};

Node* negation(Node* node) { return new Node('~', nullptr, node); }

Node* conjunction(Node* leftNode, Node* rightNode) {
    return new Node('*', leftNode, rightNode);
}

Node* disjunction(Node* leftNode, Node* rightNode) {
    return new Node('+', leftNode, rightNode);
}

Node* implication(Node* leftNode, Node* rightNode) {
    return new Node('>', leftNode, rightNode);
}

inline bool isOperator(char c) {
    return c == '~' || c == '+' || c == '*' || c == '>';
}

inline int getPrecedence(char op) {
    switch (op) {
        case '>':
            return 1;
        case '+':
            return 2;
        case '*':
            return 3;
        case '~':
            return 4;
        default:
            return 0;  // operands / parentheses
    }
}

inline void destroyParseTree(Node* root) {
    if (root == nullptr) return;

    destroyParseTree(root->left);
    destroyParseTree(root->right);

    delete root;
}

#endif  // PARSETREE_NODE_H
