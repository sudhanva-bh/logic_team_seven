#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include <cctype>

#include "../common/parsetree_node.h"

// Forward declaration for the main function
std::vector<int> infixToPrefix(const std::string& infix);

namespace {
/**
 * @brief Recursively builds a parse tree from a prefix token vector.
 *
 * @param prefix The vector of integer tokens in prefix order.
 * @param index The current position in the token vector (passed by reference).
 * @return Node* The root of the constructed subtree.
 */
Node* buildTreeFromPrefixRecursive(const std::vector<int>& prefix, int& index) {
    if (index >= prefix.size()) {
        return nullptr;
    }

    // Get the current token and advance the index
    int token = prefix[index++];
    Node* node = new Node(token);

    // If the token is an operator, recursively build its children
    if (isOperator(token)) {
        if (token == NEG) {
            // Negation is a unary operator with one child (right)
            node->right = buildTreeFromPrefixRecursive(prefix, index);
        } else {
            // Other operators (IMP, DIS, CON) are binary
            node->left = buildTreeFromPrefixRecursive(prefix, index);
            node->right = buildTreeFromPrefixRecursive(prefix, index);
        }
    }
    // If the token is an operand (positive int), it's a leaf node.

    return node;
}
}  // namespace

/**
 * @brief Constructs a parse tree from a prefix expression token vector.
 *
 * @param prefix A vector of integers representing the prefix expression.
 * @return Node* A pointer to the root of the generated parse tree.
 */
Node* prefixToParseTree(const std::vector<int>& prefix) {
    int index = 0;
    return buildTreeFromPrefixRecursive(prefix, index);
}


// --- Utility functions for a complete example ---

/**
 * @brief Helper function to print the parse tree (in-order traversal) for verification.
 */
void printTreeInorder(Node* root) {
    if (root == nullptr) {
        return;
    }

    if (isOperator(root->data)) {
        std::cout << "(";
    }

    printTreeInorder(root->left);

    if (root->data > 0) {
        std::cout << "x" << root->data;
    } else {
        switch (root->data) {
            case IMP: std::cout << " > "; break;
            case DIS: std::cout << " + "; break;
            case CON: std::cout << " * "; break;
            case NEG: std::cout << "~"; break;
        }
    }

    printTreeInorder(root->right);

    if (isOperator(root->data)) {
        std::cout << ")";
    }
}


/**
 * @brief Converts an infix logical expression string to a prefix token vector.
 * (This function is included from the previous step to make this a runnable example)
 */
std::vector<int> infixToPrefix(const std::string& infix) {
    std::vector<int> reversedTokens;
    for (int i = infix.length() - 1; i >= 0; --i) {
        char c = infix[i];
        if (isspace(c)) continue;
        if (isdigit(c)) {
            std::string numStr;
            while (i >= 0 && isdigit(infix[i])) {
                numStr += infix[i--];
            }
            if (i >= 0 && infix[i] == 'x') {
                std::reverse(numStr.begin(), numStr.end());
                reversedTokens.push_back(std::stoi(numStr));
            } else {
                i += numStr.length();
            }
        } else if (c == ')') {
            reversedTokens.push_back(L_PAREN);
        } else if (c == '(') {
            reversedTokens.push_back(R_PAREN);
        } else if (c == '>') {
            reversedTokens.push_back(IMP);
        } else if (c == '+') {
            reversedTokens.push_back(DIS);
        } else if (c == '*') {
            reversedTokens.push_back(CON);
        } else if (c == '~') {
            reversedTokens.push_back(NEG);
        }
    }

    std::stack<int> opStack;
    std::vector<int> postfixTokens;
    for (int token : reversedTokens) {
        if (token > 0) {
            postfixTokens.push_back(token);
        } else if (token == L_PAREN) {
            opStack.push(token);
        } else if (token == R_PAREN) {
            while (!opStack.empty() && opStack.top() != L_PAREN) {
                postfixTokens.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop();
        } else if (isOperator(token)) {
            while (!opStack.empty() && opStack.top() != L_PAREN &&
                   getPrecedence(token) < getPrecedence(opStack.top())) {
                postfixTokens.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
        }
    }
    while (!opStack.empty()) {
        postfixTokens.push_back(opStack.top());
        opStack.pop();
    }
    std::reverse(postfixTokens.begin(), postfixTokens.end());
    return postfixTokens;
}