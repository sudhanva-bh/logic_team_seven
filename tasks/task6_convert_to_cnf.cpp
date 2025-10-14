#include <algorithm>
#include <stack>
#include <string>

#include "../common/parsetree_node.h"

using namespace std;

namespace {
Node* impl_free_recursive(Node* currentNode) {
    if (!currentNode) return nullptr;  // Base case

    char token = currentNode->data;

    if (token == '>') {
        Node* leftImplFree = impl_free_recursive(currentNode->left);
        Node* rightImplFree = impl_free_recursive(currentNode->right);

        Node* notLeft = new Node('~', nullptr, leftImplFree);
        return new Node('+', notLeft, rightImplFree);
    }

    if (token == '~') {
        return new Node('~', nullptr, impl_free_recursive(currentNode->right));
    }

    if (token == '*' || token == '+') {
        return new Node(token, impl_free_recursive(currentNode->left),
                        impl_free_recursive(currentNode->right));
    }

    return new Node(token);
}
}  // namespace

Node* impl_free(Node* rootNode) { return impl_free_recursive(rootNode); }