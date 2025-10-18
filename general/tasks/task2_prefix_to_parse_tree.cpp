#include <string>

#include "../common/parsetree_node.h"

using namespace std;

namespace {
Node* buildTreeFromPrefixRecursive(const string& prefix, int& index) {
    if (index == prefix.length()) {
        return nullptr;
    }

    char token = prefix[index++];
    Node* node = new Node(token);

    if (isOperator(token)) {
        if (token == '~') {
            node->right = buildTreeFromPrefixRecursive(prefix, index);
        } else {
            node->left = buildTreeFromPrefixRecursive(prefix, index);
            node->right = buildTreeFromPrefixRecursive(prefix, index);
        }
    }

    return node;
}
}  // namespace

Node* prefixToParseTree(const std::string& prefix) {
    int index = 0;
    return buildTreeFromPrefixRecursive(prefix, index);
}
