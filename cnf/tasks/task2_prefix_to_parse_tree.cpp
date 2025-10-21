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
NodeCNF* buildTreeFromPrefixTokens(const vector<string>& tokens,
                                   size_t& index) {
    if (index >= tokens.size()) return nullptr;

    string token = tokens[index++];
    if (token == "~")
        return negNodeCNF(buildTreeFromPrefixTokens(tokens, index));
    else if (token == "+")
        return disjNodeCNF(buildTreeFromPrefixTokens(tokens, index),
                           buildTreeFromPrefixTokens(tokens, index));
    else if (token == "*")
        return conjNodeCNF(buildTreeFromPrefixTokens(tokens, index),
                           buildTreeFromPrefixTokens(tokens, index));
    else if (token[0] == 'x')
        return literalCNF(stoi(token.substr(1)));
    return nullptr;
}

// Public function
NodeCNF* prefixToParseTreeCNF(const string& prefix) {
    vector<string> tokens = tokenizePrefix(prefix);
    size_t index = 0;
    return buildTreeFromPrefixTokens(tokens, index);
}
