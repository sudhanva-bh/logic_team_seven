#include <algorithm>

#include "../common/parsetree_node.h"

using namespace std;

namespace {
int computeHeightOfParseTreeRecursive(NodeCNF* currentNode) {
    if (currentNode == nullptr) {
        return 0;
    }

    // negNodeCNF (~)
    if (currentNode->data == -4) {
        return 1 + computeHeightOfParseTreeRecursive(currentNode->right);
    }
    // Binary operators (*, +, →)
    else if (isOperatorCNF(currentNode->data)) {
        int leftHeight = computeHeightOfParseTreeRecursive(currentNode->left);
        int rightHeight = computeHeightOfParseTreeRecursive(currentNode->right);
        return 1 + max(leftHeight, rightHeight);
    }
    // Literal (xN)
    else {
        return 1;  // literalCNF node contributes height 1
    }
}
}  // namespace

int computeHeightOfParseTreeCNF(NodeCNF* rootNode) {
    return computeHeightOfParseTreeRecursive(rootNode);
}