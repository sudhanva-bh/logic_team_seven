#include <string>

#include "../common/parsetree_node.h"

using namespace std;

// --- Task 2: Prefix to Parse Tree ---

/**
 * @brief Recursively builds a parse tree from a prefix expression string.
 * @param prefix The prefix expression.
 * @param index The current position in the string (passed by reference).
 * @return A pointer to the root of the constructed subtree.
 */
Node* buildTreeFromPrefixRecursive(const string& prefix, int& index) {
    if (index == prefix.length()) {
        return nullptr;
    }

    char token = prefix[index++];
    Node* node = new Node(token);

    if (isOperator(token)) {
        if (token == '~') {
            node->right = buildTreeFromPrefixRecursive(prefix, index);
        } else {
            node->left = buildTreeFromPrefixRecursive(prefix, index);
            node->right = buildTreeFromPrefixRecursive(prefix, index);
        }
    }

    return node;
}

/**
 * @brief Converts a prefix expression into a rooted binary parse tree.
 * @param prefix The prefix expression string.
 * @return A pointer to the root of the newly created parse tree.
 */
Node* prefixToParseTree(const std::string& prefix) {
    int index = 0;
    return buildTreeFromPrefixRecursive(prefix, index);
}
