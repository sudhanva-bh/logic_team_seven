#ifndef PARSETREE_NODE_H
#define PARSETREE_NODE_H

// Define integer constants for logical operators and parentheses
#define IMP -1      // Implication
#define DIS -2      // Disjunction (OR)
#define CON -3      // Conjunction (AND)
#define NEG -4      // Negation
#define L_PAREN -5  // Left parenthesis '('
#define R_PAREN -6  // Right parenthesis ')'

#include <iostream>

/**
 * @brief Structure representing a node in a parse tree.
 * Data is stored as an integer.
 * - Positive values: Operands (e.g., 1 for x1, 12 for x12).
 * - Negative values: Operator constants.
 */
struct Node {
    int data;
    Node* left;
    Node* right;

    /**
     * @brief Construct a new Node object with optional children.
     *
     * @param val Integer value to store in the node.
     * @param l Pointer to left child (default nullptr).
     * @param r Pointer to right child (default nullptr).
     */
    Node(int val, Node* l = nullptr, Node* r = nullptr)
        : data(val), left(l), right(r) {}
};

// Helper functions to create operator nodes
Node* negation(Node* node) { return new Node(NEG, nullptr, node); }
Node* conjunction(Node* leftNode, Node* rightNode) {
    return new Node(CON, leftNode, rightNode);
}
Node* disjunction(Node* leftNode, Node* rightNode) {
    return new Node(DIS, leftNode, rightNode);
}
Node* implication(Node* leftNode, Node* rightNode) {
    return new Node(IMP, leftNode, rightNode);
}

/**
 * @brief Check if an integer represents a valid operator (excluding
 * parentheses).
 * @param c Integer to check.
 * @return true if c is a logical operator, false otherwise.
 */
inline bool isOperator(int c) { return c >= NEG && c <= IMP; }

/**
 * @brief Get the precedence of an operator.
 *
 * Higher number means higher precedence.
 * Precedence: IMP (1) < DIS (2) < CON (3) < NEG (4)
 *
 * @param op Operator integer.
 * @return int Precedence of the operator.
 */
inline int getPrecedence(int op) {
    if (isOperator(op)) {
        return -op;  // NEG (-4) -> 4, CON (-3) -> 3, etc.
    }
    return 0;  // Operands / parentheses
}

/**
 * @brief Recursively delete a parse tree to free memory.
 * @param root Pointer to the root of the parse tree.
 */
inline void destroyParseTree(Node* root) {
    if (root == nullptr) return;
    destroyParseTree(root->left);
    destroyParseTree(root->right);
    delete root;
}

#endif  // PARSETREE_NODE_H
