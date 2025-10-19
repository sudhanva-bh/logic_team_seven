/**
 * @file task4_compute_height_of_parse_tree.cpp
 * @brief Computes the height of a logical expression parse tree.
 *
 * @details The height is defined as the number of edges on the longest
 * path from the root node to a leaf node. Operands (leaves)
 * are defined to have a height of 0.
 */

#include "../common/parsetree_node.h" // Assumed to contain Node, isOperator

using namespace std;

/**
 * @namespace
 * @brief Contains internal helper functions for tree traversal.
 */
namespace {
/**
 * @brief Recursively computes the height of a sub-tree.
 *
 * @internal
 * @param currentNode The root of the sub-tree to compute the height for.
 * @return The height of the given sub-tree.
 */
int computeHeightOfParseTreeRecursive(Node* currentNode) {
    if (currentNode == nullptr) {
        return 0; // Height of a null tree is 0
    }

    char token = currentNode->data;

    if (token == '~') {
        // Unary operator: 1 + height of its child
        return 1 + computeHeightOfParseTreeRecursive(currentNode->right);
    } else if (isOperator(token)) {
        // Binary operator: 1 + max height of its children
        int leftHeight = computeHeightOfParseTreeRecursive(currentNode->left);
        int rightHeight = computeHeightOfParseTreeRecursive(currentNode->right);
        return 1 + max(leftHeight, rightHeight);
    } else {
        // Operand (leaf): height is 0
        return 0;
    }
}
}  // namespace

/**
 * @brief Public interface to compute the height of a parse tree.
 *
 * @param rootNode A pointer to the root of the parse tree.
 * @return The total height of the tree.
 */
int computeHeightOfParseTree(Node* rootNode) {
    return computeHeightOfParseTreeRecursive(rootNode);
}