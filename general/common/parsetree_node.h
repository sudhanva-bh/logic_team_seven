/**
 * @file parsetree_node.h
 * @brief Defines the core Node struct and all helper functions.
 *
 * @details This is the central header for the parse tree. It defines the
 * Node data structure and provides inline helper functions for creating,
 * copying, destroying, and analyzing nodes and their properties.
 */

#ifndef PARSETREE_NODE_H
#define PARSETREE_NODE_H

#include <iostream> // Included from your original file

/**
 * @struct Node
 * @brief A single node in the logical expression parse tree.
 *
 * @details Each node holds a single character (`data`) which is either
 * an operator or an operand. It also holds pointers to its
 * left and right children.
 */
struct Node
{
    char data;   ///< The operator (e.g., '+', '*') or operand (e.g., 'A').
    Node *left;  ///< Pointer to the left child (used for binary ops).
    Node *right; ///< Pointer to the right child (used for binary and unary ops).

    /**
     * @brief Construct a new Node object.
     * @param val The character for the node's data.
     * @param l Optional. Pointer to the left child.
     * @param r Optional. Pointer to the right child.
     */
    Node(char val, Node *l = nullptr, Node *r = nullptr)
        : data(val), left(l), right(r) {}
};

/**
 * @brief Creates a deep copy of a node and all its descendants.
 * @param src The node to copy.
 * @return A pointer to the root of the new, copied tree.
 */
inline Node *copyNode(const Node *src)
{
    if (!src)
        return nullptr;
    Node *out = new Node(src->data);
    out->left = copyNode(src->left);
    out->right = copyNode(src->right);
    return out;
}

/**
 * @brief Creates a new leaf node (a literal/operand).
 * @param s The character for the literal (e.g., 'A').
 * @return A new leaf Node.
 */
inline Node *literal(char s) { return new Node(s); }

/**
 * @brief Creates a new negation (NOT) node.
 * @param node The child node to be negated.
 * @return A new '~' Node.
 */
inline Node *negation(Node *node) { return new Node('~', nullptr, node); }

/**
 * @brief Creates a new conjunction (AND) node.
 * @param leftNode The left child.
 * @param rightNode The right child.
 * @return A new '*' Node.
 */
inline Node *conjunction(Node *leftNode, Node *rightNode)
{
    return new Node('*', leftNode, rightNode);
}

/**
 * @brief Creates a new disjunction (OR) node.
 * @param leftNode The left child.
 * @param rightNode The right child.
 * @return A new '+' Node.
 */
inline Node *disjunction(Node *leftNode, Node *rightNode)
{
    return new Node('+', leftNode, rightNode);
}

/**
 * @brief Creates a new implication (IMPLIES) node.
 * @param leftNode The left child (antecedent).
 * @param rightNode The right child (consequent).
 * @return A new '>' Node.
 */
inline Node *implication(Node *leftNode, Node *rightNode)
{
    return new Node('>', leftNode, rightNode);
}

/**
 * @brief Checks if a character is a supported logical operator.
 * @param c The character to check.
 * @return true if the character is an operator, false otherwise.
 */
inline bool isOperator(char c)
{
    return c == '~' || c == '+' || c == '*' || c == '>';
}

/**
 * @brief Checks if a node represents a literal.
 * @details A literal is defined as an atom (e.g., 'A') or
 * a negated atom (e.g., '~A'). It is not a complex expression.
 * @param node The node to check.
 * @return true if the node is a literal, false otherwise.
 */
inline bool is_literal(const Node *node)
{
    if (!node)
        return false;
    // A literal has no binary operator data
    if (node->data != '+' && node->data != '*')
    {
        // If it's a '~', its child must not be a binary operator
        if (node->data == '~')
        {
            return node->right &&
                   (node->right->data != '+' && node->right->data != '*');
        }
        // Not '~', '+', or '*', so it's a positive literal (e.g., 'A')
        return true;
    }
    return false;
}

/**
 * @brief Gets the precedence of a logical operator.
 *
 * @details Precedence order:
 * 4: ~ (NOT)
 * 3: * (AND)
 * 2: + (OR)
 * 1: > (IMPLIES)
 *
 * @param op The operator character.
 * @return An integer representing the operator's precedence.
 */
inline int getPrecedence(char op)
{
    switch (op)
    {
    case '>':
        return 1;
    case '+':
        return 2;
    case '*':
        return 3;
    case '~':
        return 4;
    default:
        return 0; // operands / parentheses
    }
}

/**
 * @brief Recursively deletes a node and all its children to free memory.
 * @param root The root of the tree/sub-tree to delete.
 */
inline void destroyParseTree(Node *root)
{
    if (root == nullptr)
        return;

    destroyParseTree(root->left);
    destroyParseTree(root->right);

    delete root;
}

#endif // PARSETREE_NODE_H
