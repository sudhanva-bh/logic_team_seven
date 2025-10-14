#include <algorithm>
#include <stack>
#include <string>

#include "../common/parsetree_node.h"

using namespace std;

// void freeTree(Node* node) {
//     if (!node) return;
//     freeTree(node->left);
//     freeTree(node->right);
//     delete node;
// }

namespace {

Node* impl_free(Node* currentNode) {
    if (!currentNode) return nullptr;

    Node* left = impl_free(currentNode->left);
    Node* right = impl_free(currentNode->right);

    Node* result = nullptr;

    switch (currentNode->data) {
        case '>':
            result = disjunction(negation(left), right);
            break;
        case '~':
            result = negation(right);
            break;
        case '*':
            result = conjunction(left, right);
            break;
        case '+':
            result = disjunction(left, right);
            break;
        default:
            result = new Node(currentNode->data);
            break;
    }

    delete currentNode;
    return result;
}

Node* nnf(Node* currentNode) {
    if (!currentNode) return nullptr;

    Node* result = nullptr;

    switch (currentNode->data) {
        case '+':
            result = disjunction(nnf(currentNode->left), nnf(currentNode->right));
            break;
        case '*':
            result = conjunction(nnf(currentNode->left), nnf(currentNode->right));
            break;
        case '~': {
            Node* innerNode = currentNode->right;
            switch (innerNode->data) {
                case '~':
                    result = nnf(innerNode->right);
                    break;
                case '+':
                    result = conjunction(
                        nnf(negation(innerNode->left)),
                        nnf(negation(innerNode->right)));
                    break;
                case '*':
                    result = disjunction(
                        nnf(negation(innerNode->left)),
                        nnf(negation(innerNode->right)));
                    break;
                default:
                    result = negation(innerNode);
                    break;
            }
            break;
        }
        default:
            result = new Node(currentNode->data);
            break;
    }

    delete currentNode;
    return result;
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
    if (!currentNode) return nullptr;

    Node* result = nullptr;

    switch (currentNode->data) {
        case '*':
            result = conjunction(cnf(currentNode->left), cnf(currentNode->right));
            break;
        case '+':
            result = distr(cnf(currentNode->left), cnf(currentNode->right));
            break;
        default:
            result = currentNode;
            break;
    }

    return result;
}

}

Node* computeCnfFromParseTree(Node* rootNode) {
    if (!rootNode) return nullptr;
    Node* implFreeTree = impl_free(rootNode);
    Node* nnfTree = nnf(implFreeTree);
    Node* cnfTree = cnf(nnfTree);
    return cnfTree;
}
