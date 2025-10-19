#include <string>
#include "../common/parsetree_node.h"

Node* prefixToParseTree(const std::string& prefix,int& index){
    if(index == prefix.length()) return nullptr;

    char currentChar = prefix[index++];
    Node* node = new Node(currentChar);
    if(currentChar == '~'){
        node->right = prefixToParseTree(prefix,index);
    }
    else if(currentChar == '+' || currentChar == '*' || currentChar == '^'){
        node->left = prefixToParseTree(prefix,index);
        node->right = prefixToParseTree(prefix,index);
    }

    return node;
}