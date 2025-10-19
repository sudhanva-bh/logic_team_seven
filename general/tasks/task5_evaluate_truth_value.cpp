/**
 * @file task5_evaluate_truth_value.cpp
 * @brief Evaluates the truth value of a parse tree given atom assignments.
 *
 * @details This file provides a function to compute the boolean result
 * of a logical expression tree based on a given mapping of atomic
 * propositions (variables) to their T/F values.
 */

#include <algorithm>
#include <map>
#include <stack>
#include <string>

#include "../common/parsetree_node.h" // Assumed to contain Node

using namespace std;

/**
 * @namespace
 * @brief Contains internal helper functions for tree evaluation.
 */
namespace {
/**
 * @brief Recursively evaluates the truth value of a sub-tree.
 *
 * @internal
 * @param currentNode The root of the sub-tree to evaluate.
 * @param truthValues A map containing the boolean value for each atom.
 * @return The boolean result of the sub-tree's expression.
 */
bool getTruthValueRecursion(Node* currentNode,
                            const map<char, bool>& truthValues) {
    if (currentNode == nullptr) {
        // This case should ideally not be hit in a well-formed tree
        // but returning true (identity for AND) is a safe default.
        return true;
    }

    char token = currentNode->data;

    switch (token) {
        case '~': // NOT
            return !getTruthValueRecursion(currentNode->right, truthValues);
        case '*': // AND
            return getTruthValueRecursion(currentNode->left, truthValues) &&
                   getTruthValueRecursion(currentNode->right, truthValues);
        case '+': // OR
            return getTruthValueRecursion(currentNode->left, truthValues) ||
                   getTruthValueRecursion(currentNode->right, truthValues);
        case '>': // IMPLIES (A > B is equivalent to !A or B)
            return !getTruthValueRecursion(currentNode->left, truthValues) ||
                   getTruthValueRecursion(currentNode->right, truthValues);
        default:
            // Operand (atom): Look up its value in the map
            return truthValues.at(token);
    }
}
}  // namespace

/**
 * @brief Public interface to evaluate the truth value of a parse tree.
 *
 * @param rootNode A pointer to the root of the parse tree.
 * @param truthValues A constant map where keys are atomic propositions (char)
 * and values are their assigned boolean truth values.
 * @return The final boolean result of the entire expression.
 */
bool evaluateTruthValue(Node* rootNode, const map<char, bool> truthValues) {
    return getTruthValueRecursion(rootNode, truthValues);
}