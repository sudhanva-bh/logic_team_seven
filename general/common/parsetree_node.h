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

inline Node* copyNode(const Node* src) {
    if (!src) return nullptr;
    Node* out = new Node(src->data);
    out->left = copyNode(src->left);
    out->right = copyNode(src->right);
    return out;
}

Node* literal(char s) { return new Node(s); }

Node* negationNode(Node* node) { return new Node('~', nullptr, node); }

Node* conjunctionNode(Node* leftNode, Node* rightNode) {
    return new Node('*', leftNode, rightNode);
}

Node* disjunctionNode(Node* leftNode, Node* rightNode) {
    return new Node('+', leftNode, rightNode);
}

Node* implication(Node* leftNode, Node* rightNode) {
    return new Node('>', leftNode, rightNode);
}

inline bool isOperator(char c) {
    return c == '~' || c == '+' || c == '*' || c == '>';
}

inline bool is_literal(const Node* node) {
    if (!node) return false;
    // A literal has no operator data
    if (node->data != '+' && node->data != '*') {
        // If it's a '~', its child must not be an operator
        if (node->data == '~') {
            return node->right &&
                   (node->right->data != '+' && node->right->data != '*');
        }
        // Not '~', '+', or '*', so it's a positive literal (e.g., 'A')
        return true;
    }
    return false;
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
