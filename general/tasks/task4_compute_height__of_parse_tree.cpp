/**
 * @file task4_compute_height_of_parse_tree.cpp
 * @brief Computes the height of a logical expression parse tree.
 *
 * @details
 * The height is defined as the number of edges on the longest
 * path from the root node to a leaf node.
 * Operands (leaves) are defined to have a height of 0.
 */

#include "../common/parsetree_node.h" ///< Contains Node struct and isOperator()

using namespace std;

/**
 * @namespace internal
 * @brief Helper functions for parse tree height calculation.
 */
namespace internal
{

    /**
     * @brief Recursively computes the height of a sub-tree.
     *
     * @param currentNode The root of the sub-tree to compute the height for.
     * @return The height of the given sub-tree.
     */
    int computeHeightOfParseTreeRecursive(Node *currentNode)
    {
        if (currentNode == nullptr)
        {
            return 0; ///< Height of an empty tree is 0
        }

        char token = currentNode->data;

        if (token == '~')
        {
            // Unary operator: height = 1 + height of its single child
            return 1 + computeHeightOfParseTreeRecursive(currentNode->right);
        }
        else if (isOperator(token))
        {
            // Binary operator: height = 1 + max height of children
            int leftHeight = computeHeightOfParseTreeRecursive(currentNode->left);
            int rightHeight = computeHeightOfParseTreeRecursive(currentNode->right);
            return 1 + max(leftHeight, rightHeight);
        }
        else
        {
            // Operand (leaf node) height is 0
            return 0;
        }
    }

} // namespace internal

/**
 * @brief Computes the height of the entire parse tree.
 *
 * This is the public interface that calls the recursive helper function.
 *
 * @param rootNode Pointer to the root node of the parse tree.
 * @return The height of the parse tree.
 */
int computeHeightOfParseTree(Node *rootNode)
{
    return internal::computeHeightOfParseTreeRecursive(rootNode);
}
