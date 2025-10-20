#include <cctype>
#include <iostream>
#include <string>
#include <vector>

#include "../common/parsetree_node.h"

using namespace std;

// Tokenize prefix string into multi-character operands and operators
vector<string> tokenizePrefix(const string& prefix) {
    vector<string> tokens;
    for (size_t i = 0; i < prefix.size();) {
        char c = prefix[i];
        if (c == '+' || c == '*' || c == '~') {
            tokens.push_back(string(1, c));
            ++i;
        } else if (c == 'x') {
            size_t j = i + 1;
            while (j < prefix.size() && isdigit(prefix[j])) j++;
            tokens.push_back(prefix.substr(i, j - i));
            i = j;
        } else
            i++;
    }
    return tokens;
}

// Recursive tree builder
Node* buildTreeFromPrefixTokens(const vector<string>& tokens, size_t& index) {
    if (index >= tokens.size()) return nullptr;

    string token = tokens[index++];
    if (token == "~")
        return negNode(buildTreeFromPrefixTokens(tokens, index));
    else if (token == "+")
        return disNode(buildTreeFromPrefixTokens(tokens, index),
                       buildTreeFromPrefixTokens(tokens, index));
    else if (token == "*")
        return conNode(buildTreeFromPrefixTokens(tokens, index),
                       buildTreeFromPrefixTokens(tokens, index));
    else if (token[0] == 'x')
        return literal(stoi(token.substr(1)));
    return nullptr;
}

// Public function
Node* prefixToParseTree(const string& prefix) {
    vector<string> tokens = tokenizePrefix(prefix);
    size_t index = 0;
    return buildTreeFromPrefixTokens(tokens, index);
}

// Optional: print tree inorder for testing
void printInorder(Node* node) {
    if (!node) return;
    if (node->left) printInorder(node->left);
    if (node->data > 0)
        cout << "x" << node->data;
    else {
        switch (node->data) {
            case -2:
                cout << "+";
                break;
            case -3:
                cout << "*";
                break;
            case -4:
                cout << "~";
                break;
            default:
                break;
        }
    }
    if (node->right) printInorder(node->right);
}
