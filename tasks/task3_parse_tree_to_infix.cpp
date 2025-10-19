#include <string>

#include "../common/parsetree_node.h"

using namespace std;

namespace {
string buildParseTreeToInfixRecursive(Node* currentNode, bool isRoot) {
    if (currentNode == nullptr) {
        return "";
    }

    char token = currentNode->data;

    if (token == '~') {
        return "~" + buildParseTreeToInfixRecursive(currentNode->right, false);
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
}  // namespace

string parseTreeToInfix(Node* rootNode) {
    return buildParseTreeToInfixRecursive(rootNode, true);
}
