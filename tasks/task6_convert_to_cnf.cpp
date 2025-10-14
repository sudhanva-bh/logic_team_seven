#include <algorithm>
#include <stack>
#include <string>

#include "../common/parsetree_node.h"

using namespace std;

namespace {
Node* impl_free(Node* currentNode) {
    if (!currentNode) return nullptr;

    Node* left = impl_free(currentNode->left);
    Node* right = impl_free(currentNode->right);

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

Node* nnf(Node* currentNode) {
    switch (currentNode->data) {
        case '+':
            return disjunction(nnf(currentNode->left), nnf(currentNode->right));
        case '*':
            return conjunction(nnf(currentNode->left), nnf(currentNode->right));
        case '~': {
            Node* innerNode = currentNode->right;
            switch (innerNode->data) {
                case '~':
                    return nnf(innerNode->right);

                case '+':
                    return conjunction(nnf(negation(innerNode->left)),
                                       nnf(negation(innerNode->right)));

                case '*':
                    return disjunction(nnf(negation(innerNode->left)),
                                       nnf(negation(innerNode->right)));

                default:
                    return negation(innerNode);
            }
        }
        default:
            return new Node(currentNode->data);
    }
}

Node* distr(Node* node1, Node* node2) {
    if (node1->data == '*') {
        return conjunction(distr(node1->left, node2),
                           distr(node1->right, node2));
    } else if (node2->data == '*') {
        return conjunction(distr(node1, node2->left),
                           distr(node1, node2->right));
    } else {
        return disjunction(node1, node2);
    }
}

Node* cnf(Node* currentNode) {
    switch (currentNode->data) {
        case '*':
            return conjunction(cnf(currentNode->left), cnf(currentNode->right));
        case '+':
            return distr(cnf(currentNode->left), cnf(currentNode->right));

        default:
            return currentNode;
    }
}
}  // namespace

Node* computeCnfFromParseTree(Node* rootNode) {
    return cnf(nnf(impl_free(rootNode)));
}