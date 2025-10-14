#include <algorithm>
#include <stack>
#include <string>

#include "../common/parsetree_node.h"

using namespace std;

namespace {
Node* impl_free_recursive(Node* currentNode) {
    if (!currentNode) return nullptr;

    Node* left = impl_free_recursive(currentNode->left);
    Node* right = impl_free_recursive(currentNode->right);

    switch (currentNode->data) {
        case '>':
            return disjunction(negation(left), right);  // (~A + B)
        case '~':
            return negation(right);
        case '*':
            return conjunction(left, right);
        case '+':
            return disjunction(left, right);
        default:
            return new Node(currentNode->data);
    }
}
}  // namespace

Node* impl_free(Node* rootNode) { return impl_free_recursive(rootNode); }

namespace {
Node* nnf_recursion(Node* currentNode) {
    switch (currentNode->data) {
        case '+':
            return disjunction(nnf_recursion(currentNode->left),
                               nnf_recursion(currentNode->right));
        case '*':
            return conjunction(nnf_recursion(currentNode->left),
                               nnf_recursion(currentNode->right));
        case '~': {
            Node* innerNode = currentNode->right;

            if (innerNode->data == '~') {
                return nnf_recursion(innerNode->right);
            } else if (innerNode->data == '+') {
                return conjunction(nnf_recursion(negation(innerNode->left)),
                                   nnf_recursion(negation(innerNode->right)));
            } else if (innerNode->data == '*') {
                return disjunction(nnf_recursion(negation(innerNode->left)),
                                   nnf_recursion(negation(innerNode->right)));
            } else {
                return negation(innerNode);
            }
        }
        default:
            return new Node(currentNode->data);
    }
}
}  // namespace

Node* nnf(Node* rootNode) { return nnf_recursion(rootNode); }