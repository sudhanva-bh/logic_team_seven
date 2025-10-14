#include "../common/parsetree_node.h"

using namespace std;

// --- Task 4: Computing the Height of a Parse Tree ---

/**
 * @brief Helper function that recursively computes the height (in edges)
 * of a parse tree.
 *
 * The height is defined as the number of edges on the longest path
 * from the current node down to a leaf.
 *
 * @param currentNode Pointer to the current node being processed.
 * @return int Height of the current subtree in edges.
 */
int computeHeightOfParseTreeRecursive(Node* currentNode) {
    if (currentNode == nullptr) {
        return 0;
    }

    char token = currentNode->data;

    if (token == '~') {
        return 1 + computeHeightOfParseTreeRecursive(currentNode->right);
    } else if (isOperator(token)) {
        int leftHeight = computeHeightOfParseTreeRecursive(currentNode->left);
        int rightHeight = computeHeightOfParseTreeRecursive(currentNode->right);
        return 1 + max(leftHeight, rightHeight);
    } else {
        return 0;
    }
}

/**
 * @brief Computes the height (in edges) of a parse tree rooted at the given
 * node.
 *
 * This is the wrapper function that starts the recursive computation
 * of height from the root node.
 *
 * @param rootNode Pointer to the root of the parse tree.
 * @return int Height of the parse tree in terms of edges.
 */
int computeHeightOfParseTree(Node* rootNode) {
    return computeHeightOfParseTreeRecursive(rootNode);
}
