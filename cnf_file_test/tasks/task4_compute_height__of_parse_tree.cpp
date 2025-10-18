#include "../common/parsetree_node.h"

using namespace std;

namespace {
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
}  // namespace

int computeHeightOfParseTree(Node* rootNode) {
    return computeHeightOfParseTreeRecursive(rootNode);
}
