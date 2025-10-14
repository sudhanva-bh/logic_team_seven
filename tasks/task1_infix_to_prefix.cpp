#include <algorithm>
#include <stack>
#include <string>

#include "../common/parsetree_node.h"

using namespace std;

// --- Task 1: Infix to Prefix Conversion ---

/**
 * @brief Converts an infix propositional logic expression to a prefix
 * expression.
 * @param infix The fully parenthesized infix expression string. [cite: 3]
 * @return The equivalent prefix expression string.
 */
string infixToPrefix(string infix) {
    // Step 1: Reverse the infix expression.
    reverse(infix.begin(), infix.end());

    // Step 2: Swap opening and closing parentheses.
    for (size_t i = 0; i < infix.length(); ++i) {
        if (infix[i] == '(') {
            infix[i] = ')';
        } else if (infix[i] == ')') {
            infix[i] = '(';
        }
    }

    // Step 3: Convert the modified infix string to postfix.
    stack<char> opStack;
    string postfix;

    for (char token : infix) {
        // If token is an operand (a propositional atom), add it to the output.
        if (isalnum(token)) {
            postfix += token;
        }
        // If token is an opening parenthesis, push it onto the stack.
        else if (token == '(') {
            opStack.push(token);
        }
        // If token is a closing parenthesis, pop operators until '(' is found.
        else if (token == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.pop();  // Pop the opening parenthesis '('
        }
        // If token is an operator.
        else if (isOperator(token)) {
            while (!opStack.empty() && opStack.top() != '(' &&
                   getPrecedence(token) < getPrecedence(opStack.top())) {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.push(token);
        }
    }

    // Pop any remaining operators from the stack.
    while (!opStack.empty()) {
        postfix += opStack.top();
        opStack.pop();
    }

    // Step 4: Reverse the postfix expression to get the prefix expression.
    reverse(postfix.begin(), postfix.end());

    return postfix;
}
