#include <algorithm>

#include "../common/parsetree_node.h"

using namespace std;

namespace {
int computeHeightOfParseTreeRecursive(Node* currentNode) {
    if (currentNode == nullptr) {
        return 0;
    }

    // negNode (~)
    if (currentNode->data == -4) {
        return 1 + computeHeightOfParseTreeRecursive(currentNode->right);
    }
    // Binary operators (*, +, →)
    else if (isOperator(currentNode->data)) {
        int leftHeight = computeHeightOfParseTreeRecursive(currentNode->left);
        int rightHeight = computeHeightOfParseTreeRecursive(currentNode->right);
        return 1 + max(leftHeight, rightHeight);
    }
    // Literal (xN)
    else {
        return 1;  // literal node contributes height 1
    }
}
}  // namespace

int computeHeightOfParseTree(Node* rootNode) {
    return computeHeightOfParseTreeRecursive(rootNode);
}