/**
 * @file task3_parse_tree_to_infix.cpp
 * @brief Converts a parse tree back into an infix notation string.
 *
 * @details This file provides a function to traverse an expression tree
 * and reconstruct the standard, human-readable infix string,
 * correctly placing parentheses to maintain operator precedence.
 */

#include <string>

#include "../common/parsetree_node.h" // Assumed to contain Node, isOperator

using namespace std;

/**
 * @namespace
 * @brief Contains internal helper functions for tree traversal.
 */
namespace
{
    /**
     * @brief Recursively builds an infix string from a parse tree.
     *
     * @details Traverses the tree and builds the string. It adds parentheses
     * around sub-expressions (any non-root binary operation) to ensure
     * that the original operator precedence is preserved.
     *
     * @internal
     * @param currentNode The current node in the tree traversal.
     * @param isRoot True if this node is the root of the entire tree
     * (to avoid unnecessary outer parentheses), false otherwise.
     * @return A string representing the infix expression of the sub-tree.
     */
    string buildParseTreeToInfixRecursive(Node *currentNode, bool isRoot)
    {
        if (currentNode == nullptr)
        {
            return "";
        }

        char token = currentNode->data;

        if (token == '~')
        {
            // Parentheses are not needed for the right-hand side of NOT
            return "~" + buildParseTreeToInfixRecursive(currentNode->right, false);
        }
        else if (isOperator(token))
        {
            string left = buildParseTreeToInfixRecursive(currentNode->left, false);
            string right =
                buildParseTreeToInfixRecursive(currentNode->right, false);
            string result = left + token + right;

            // Add parentheses unless it's the top-level expression
            return isRoot ? result : "(" + result + ")";
        }
        else
        {
            // Operand (leaf node)
            return string(1, token);
        }
    }
} // namespace

/**
 * @brief Public interface to convert a parse tree to an infix string.
 *
 * @param rootNode A pointer to the root of the parse tree.
 * @return A string containing the expression in infix notation.
 */
string parseTreeToInfix(Node *rootNode)
{
    return buildParseTreeToInfixRecursive(rootNode, true);
}