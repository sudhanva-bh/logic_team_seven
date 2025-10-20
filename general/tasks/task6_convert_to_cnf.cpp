#include <algorithm>
#include <iostream>  // for debug
#include <stack>
#include <string>

#include "../common/parsetree_node.h"

using namespace std;

namespace {

Node* impl_free(Node* currentNode) {
    if (!currentNode) return nullptr;

    Node* left = impl_free(currentNode->left);
    Node* right = impl_free(currentNode->right);

    Node* result = nullptr;

    switch (currentNode->data) {
        case '>':
            result = disNode(negNode(left), right);
            break;
        case '~':
            result = negNode(right);
            break;
        case '*':
            result = conNode(left, right);
            break;
        case '+':
            result = disNode(left, right);
            break;
        default:
            result = new Node(currentNode->data);
            break;
    }
    return result;
}

Node* nnf(Node* currentNode) {
    if (!currentNode) return nullptr;

    Node* result = nullptr;

    switch (currentNode->data) {
        case '+':
            result = disNode(nnf(currentNode->left), nnf(currentNode->right));
            break;
        case '*':
            result = conNode(nnf(currentNode->left), nnf(currentNode->right));
            break;
        case '~': {
            Node* innerNode = currentNode->right;
            switch (innerNode->data) {
                case '~':
                    result = nnf(innerNode->right);
                    break;
                case '+':
                    result = conNode(nnf(negNode(innerNode->left)),
                                     nnf(negNode(innerNode->right)));
                    break;
                case '*':
                    result = disNode(nnf(negNode(innerNode->left)),
                                     nnf(negNode(innerNode->right)));
                    break;
                default:
                    result = negNode(innerNode);
                    break;
            }
            break;
        }
        default:
            result = new Node(currentNode->data);
            break;
    }
    return result;
}

Node* distr(Node* node1, Node* node2, int depth = 0) {
    if (!node1 || !node2) return nullptr;

    const int MAX_DEPTH = 2000;  // tune as needed
    if (depth > MAX_DEPTH) {
        "disNode copy\n";
        return disNode(copyNode(node1), copyNode(node2));
    }

    // If left is a conNode, distribute: (A*B)+C => (A+C)*(B+C)
    if (node1->data == '*') {
        Node* a = distr(node1->left, node2, depth + 1);
        Node* b = distr(node1->right, node2, depth + 1);
        return conNode(a, b);
    }

    // If right is a conNode, distribute: A+(B*C) => (A+B)*(A+C)
    if (node2->data == '*') {
        Node* a = distr(node1, node2->left, depth + 1);
        Node* b = distr(node1, node2->right, depth + 1);
        return conNode(a, b);
    }

    // Neither side is conNode -> no distribution needed. Return new
    // disNode of copies
    return disNode(copyNode(node1), copyNode(node2));
}

Node* cnf(Node* currentNode, int depth = 0) {
    if (!currentNode) return nullptr;

    const int MAX_DEPTH = 2000;
    string indent(depth * 2, ' ');

    if (depth > MAX_DEPTH) {
        return copyNode(currentNode);
    }

    Node* result = nullptr;

    switch (currentNode->data) {
        case '*': {
            Node* leftCNF = cnf(currentNode->left, depth + 1);
            Node* rightCNF = cnf(currentNode->right, depth + 1);
            result = conNode(leftCNF, rightCNF);
            break;
        }
        case '+': {
            Node* leftCNF = cnf(currentNode->left, depth + 1);
            Node* rightCNF = cnf(currentNode->right, depth + 1);
            result = distr(leftCNF, rightCNF, depth + 1);
            break;
        }
        default:
            result = copyNode(currentNode);  // Always return a safe copy
            break;
    }

    return result;
}

}  // namespace

Node* computeCnfFromParseTree(Node* rootNode) {
    if (!rootNode) return nullptr;

    Node* implFreeTree = impl_free(rootNode);

    Node* nnfTree = nnf(implFreeTree);

    Node* cnfTree = cnf(nnfTree);

    return cnfTree;
}
