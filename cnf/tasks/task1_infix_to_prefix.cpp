#include <algorithm>
#include <cctype>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

bool isOperator(const string &token) {
    return token == "+" || token == "*" || token == "~";
}
int getPrecedence(const string &op) {
    return (op == "*") ? 2 : (op == "+") ? 1 : 3;
}

// Tokenize infix into operands/operators/parentheses
vector<string> tokenize(const string &expr) {
    vector<string> tokens;
    for (size_t i = 0; i < expr.size();) {
        char c = expr[i];
        if (c == '(' || c == ')' || c == '+' || c == '*' || c == '~') {
            tokens.push_back(string(1, c));
            ++i;
        } else {
            size_t j = i;
            while (j < expr.size() && expr[j] != '(' && expr[j] != ')' &&
                   expr[j] != '+' && expr[j] != '*' && expr[j] != '~')
                ++j;
            tokens.push_back(expr.substr(i, j - i));
            i = j;
        }
    }
    return tokens;
}

// Convert infix expression to prefix
string infixToPrefix(const string &infix) {
    vector<string> tokens = tokenize(infix);
    reverse(tokens.begin(), tokens.end());
    for (auto &t : tokens)
        if (t == "(")
            t = ")";
        else if (t == ")")
            t = "(";

    stack<string> opStack;
    vector<string> output;

    for (const string &tok : tokens) {
        if (!isOperator(tok) && tok != "(" && tok != ")") {
            output.push_back(tok);
        } else if (tok == "(")
            opStack.push(tok);
        else if (tok == ")") {
            while (!opStack.empty() && opStack.top() != "(") {
                output.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop();
        } else {
            while (!opStack.empty() && opStack.top() != "(" &&
                   getPrecedence(tok) < getPrecedence(opStack.top())) {
                output.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(tok);
        }
    }

    while (!opStack.empty()) {
        output.push_back(opStack.top());
        opStack.pop();
    }

    reverse(output.begin(), output.end());
    string prefix;
    for (const auto &tok : output) prefix += tok;
    return prefix;
}
