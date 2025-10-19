/**
 * @file task2_prefix_to_parse_tree.cpp
 * @brief Builds a parse tree from a prefix (Polish) notation string.
 *
 * @details This file provides the functionality to convert a prefix logical
 * expression (e.g., "*A|BC") into its corresponding binary expression tree
 * representation, which is essential for evaluation.
 */

#include <string>

#include "../common/parsetree_node.h" // Assumed to contain Node, isOperator

using namespace std;

/**
 * @namespace
 * @brief Contains internal helper functions for tree construction.
 */
namespace
{
    /**
     * @brief Recursively builds a parse tree from a prefix string.
     *
     * @details This is the internal recursive implementation. It reads one token
     * from the prefix string. If the token is an operator, it recursively
     * builds its children. If it's an operand, it returns a leaf node.
     *
     * The `index` is passed by reference and advanced as tokens are consumed.
     *
     * @internal
     * @param prefix The complete prefix expression string.
     * @param index The current position in the prefix string (passed by reference).
     * @return A pointer to the root Node of the newly constructed sub-tree.
     */
    Node *buildTreeFromPrefixRecursive(const string &prefix, int &index)
    {
        if (index == prefix.length())
        {
            return nullptr;
        }

        char token = prefix[index++];
        Node *node = new Node(token);

        if (isOperator(token))
        {
            if (token == '~')
            {
                node->right = buildTreeFromPrefixRecursive(prefix, index);
            }
            else
            {
                node->left = buildTreeFromPrefixRecursive(prefix, index);
                node->right = buildTreeFromPrefixRecursive(prefix, index);
            }
        }

        return node;
    }
} // namespace

/**
 * @brief Public interface to convert a prefix string to a parse tree.
 *
 * @details This function initializes the recursive building process.
 * It creates a new parse tree from the given prefix expression.
 *
 * @param prefix The input expression in prefix notation (e.g., "*A|BC").
 * @return A pointer to the root of the generated parse tree.
 */
Node *prefixToParseTree(const std::string &prefix)
{
    int index = 0;
    return buildTreeFromPrefixRecursive(prefix, index);
}