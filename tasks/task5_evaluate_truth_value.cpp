#include <algorithm>
#include <stack>
#include <string>

#include "../common/parsetree_node.h"

using namespace std;

/**
 * @brief Recursively computes the truth value of a logical expression tree.
 * @param currentNode Pointer to the current node being evaluated in the parse
 * tree.
 * @param truthValues A mapping of variable characters to their corresponding
 * boolean values.
 * @return bool The evaluated truth value at the current subtree.
 */
bool getTruthValueRecursion(Node* currentNode,
                            const map<char, bool> truthValues) {
    if (currentNode == nullptr) {
        return true;
    }

    char token = currentNode->data;

    switch (token) {
        case '~':
            return !getTruthValueRecursion(currentNode->right, truthValues);
            break;
        case '*':
            return getTruthValueRecursion(currentNode->left, truthValues) &&
                   getTruthValueRecursion(currentNode->right, truthValues);
            break;
        case '+':
            return getTruthValueRecursion(currentNode->left, truthValues) ||
                   getTruthValueRecursion(currentNode->right, truthValues);
        case '>':
            return !getTruthValueRecursion(currentNode->left, truthValues) ||
                   getTruthValueRecursion(currentNode->right, truthValues);
            break;
        default:
            return truthValues.at(token);
    }
}

/**
 * @brief Evaluates the truth value of a logical expression represented by a
 * parse tree.
 * @param rootNode Pointer to the root node of the parse tree.
 * @param truthValues A mapping of variable characters to their corresponding
 * boolean values.
 * @return bool The evaluated truth value of the entire logical expression.
 */
bool evaluateTruthValue(Node* rootNode, const map<char, bool> truthValues) {
    return getTruthValueRecursion(rootNode, truthValues);
}