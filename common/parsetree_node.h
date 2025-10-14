#ifndef PARSETREE_NODE_H
#define PARSETREE_NODE_H

#include <iostream>

/**
 * @brief Structure representing a node in a parse tree.
 */
struct Node {
    char data;    /**< Value stored in the node (operator or operand) */
    Node* left;   /**< Pointer to left child */
    Node* right;  /**< Pointer to right child */

    /**
     * @brief Construct a new Node object.
     * 
     * @param val Character value to store in the node.
     */
    Node(char val) : data(val), left(nullptr), right(nullptr) {}
};

/**
 * @brief Check if a character is a valid operator in the parse tree.
 * 
 * Supported operators: '~', '+', '*', '>'
 * 
 * @param c Character to check.
 * @return true if c is an operator, false otherwise.
 */
inline bool isOperator(char c) { 
    return c == '~' || c == '+' || c == '*' || c == '>'; 
}

/**
 * @brief Get the precedence of an operator.
 * 
 * Higher number means higher precedence.
 * 
 * Precedence:
 * - '>' : 1
 * - '+' : 2
 * - '*' : 3
 * - '~' : 4
 * - operands / parentheses : 0
 * 
 * @param op Operator character.
 * @return int Precedence of the operator.
 */
inline int getPrecedence(char op) {
    switch (op) {
        case '>': return 1;
        case '+': return 2;
        case '*': return 3;
        case '~': return 4;
        default: return 0; // operands / parentheses
    }
}

/**
 * @brief Recursively delete a parse tree to free memory.
 * 
 * Deletes all nodes in post-order traversal.
 * 
 * @param root Pointer to the root of the parse tree.
 */
inline void destroyParseTree(Node* root) {
    if (root == nullptr) return;

    destroyParseTree(root->left);
    destroyParseTree(root->right);

    delete root;
}

#endif // PARSETREE_NODE_H
