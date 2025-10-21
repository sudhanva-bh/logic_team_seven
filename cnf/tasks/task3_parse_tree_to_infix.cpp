#include <string>

#include "../common/parsetree_node.h"

using namespace std;

namespace {
string buildParseTreeToInfixRecursive(NodeCNF* node, bool isRoot) {
    if (!node) return "";

    // negNodeCNF
    if (node->data == -4) {
        return "~" + buildParseTreeToInfixRecursive(node->right, false);
    }
    // disjNodeCNF
    else if (node->data == -2) {
        string left = buildParseTreeToInfixRecursive(node->left, false);
        string right = buildParseTreeToInfixRecursive(node->right, false);
        string result = left + "+" + right;
        return isRoot ? result : "(" + result + ")";
    }
    // conjNodeCNF
    else if (node->data == -3) {
        string left = buildParseTreeToInfixRecursive(node->left, false);
        string right = buildParseTreeToInfixRecursive(node->right, false);
        string result = left + "*" + right;
        return isRoot ? result : "(" + result + ")";
    }
    // Literal
    else if (node->data > 0) {
        return "x" + to_string(node->data);
    }

    return "";  // fallback (should not happen)
}
}  // namespace

string parseTreeToInfixCNF(NodeCNF* rootNode) {
    return buildParseTreeToInfixRecursive(rootNode, true);
}