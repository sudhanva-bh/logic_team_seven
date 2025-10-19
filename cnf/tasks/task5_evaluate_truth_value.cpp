#include <algorithm>
#include <map>
#include <string>

#include "../common/parsetree_node.h"

using namespace std;

namespace {

bool evaluateTruthValueRecursive(Node* currentNode,
                                 const map<char, bool>& truthValues) {
    if (currentNode == nullptr) {
        return true;
    }

    int token = currentNode->data;

    switch (token) {
        case -4:
            return !evaluateTruthValueRecursive(currentNode->right,
                                                truthValues);
        case -3:
            return evaluateTruthValueRecursive(currentNode->left,
                                               truthValues) &&
                   evaluateTruthValueRecursive(currentNode->right, truthValues);
        case -2:
            return evaluateTruthValueRecursive(currentNode->left,
                                               truthValues) ||
                   evaluateTruthValueRecursive(currentNode->right, truthValues);
        case -1:
            return !evaluateTruthValueRecursive(currentNode->left,
                                                truthValues) ||
                   evaluateTruthValueRecursive(currentNode->right, truthValues);
        default:
            return truthValues.at(token);
    }
}

}  // namespace

bool evaluateTruthValue(Node* rootNode, const map<char, bool>& truthValues) {
    return evaluateTruthValueRecursive(rootNode, truthValues);
}
