#include <algorithm>
#include <map>
#include <stack>

#include <string>
#include "../common/parsetree_node.h"

using namespace std;

namespace {
bool getTruthValueRecursion(Node* currentNode,
                            const map<char, bool>& truthValues) {
    if (currentNode == nullptr) {
        return true;
    }

    char token = currentNode->data;

    switch (token) {
        case '~':
            return !getTruthValueRecursion(currentNode->right, truthValues);
        case '*':
            return getTruthValueRecursion(currentNode->left, truthValues) &&
                   getTruthValueRecursion(currentNode->right, truthValues);
        case '+':
            return getTruthValueRecursion(currentNode->left, truthValues) ||
                   getTruthValueRecursion(currentNode->right, truthValues);
        case '>':
            return !getTruthValueRecursion(currentNode->left, truthValues) ||
                   getTruthValueRecursion(currentNode->right, truthValues);
        default:
            return truthValues.at(token);
    }
}
}  // namespace

bool evaluateTruthValue(Node* rootNode, const map<char, bool> truthValues) {
    return getTruthValueRecursion(rootNode, truthValues);
}