#include <string>

#include "../common/parsetree_node.h"

using namespace std;

// --- Task 3: Parse Tree to Infix Expression ---

/**
 * @brief Helper function that recursively converts a parse tree to an infix
 * expression.
 * @param currentNode Pointer to the current node being processed.
 * @param isRoot Flag indicating whether the current node is the root of the
 * tree.
 * @return The corresponding infix expression for the current subtree.
 */
string buildParseTreeToInfixRecursive(Node* currentNode, bool isRoot) {
    if (currentNode == nullptr) {
        return "";
    }

    char token = currentNode->data;

    if (token == '~') {
        return "(~" +
               buildParseTreeToInfixRecursive(currentNode->right, false) + ")";
    } else if (isOperator(token)) {
        string left = buildParseTreeToInfixRecursive(currentNode->left, false);
        string right =
            buildParseTreeToInfixRecursive(currentNode->right, false);
        string result = left + token + right;

        return isRoot ? result : "(" + result + ")";
    } else {
        return string(1, token);
    }
}

/**
 * @brief Converts a parse tree into an infix expression string.
 *
 * This is the main wrapper function that starts the recursive process
 * from the root of the parse tree. The resulting expression omits unnecessary
 * outer parentheses.
 *
 * @param rootNode Pointer to the root node of the parse tree.
 * @return The reconstructed infix expression as a string.
 */
string parseTreeToInfix(Node* rootNode) {
    return buildParseTreeToInfixRecursive(rootNode, true);
}
