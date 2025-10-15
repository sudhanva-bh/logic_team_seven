#include <algorithm>
#include <stack>
#include <string>

#include "../common/parsetree_node.h"

using namespace std;

string infixToPrefix(string infix) {
    reverse(infix.begin(), infix.end());

    for (size_t i = 0; i < infix.length(); ++i) {
        if (infix[i] == '(') {
            infix[i] = ')';
        } else if (infix[i] == ')') {
            infix[i] = '(';
        }
    }

    stack<char> opStack;
    string postfix;

    for (char token : infix) {
        if (isalnum(token)) {
            postfix += token;
        } else if (token == '(') {
            opStack.push(token);
        } else if (token == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                postfix += opStack.top();
                opStack.pop();
            }
        } else if (isOperator(token)) {
            while (!opStack.empty() && opStack.top() != '(' &&
                   getPrecedence(token) < getPrecedence(opStack.top())) {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.push(token);
        }
    }

    while (!opStack.empty()) {
        postfix += opStack.top();
        opStack.pop();
    }

    reverse(postfix.begin(), postfix.end());

    return postfix;
}
