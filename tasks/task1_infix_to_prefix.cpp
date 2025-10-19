/**
 * @file task1_infix_to_prefix.cpp
 * @brief Converts an infix logical expression to prefix notation.
 * 
 * This file defines a function that transforms a propositional logic expression
 * from infix form (e.g., A & B) to prefix form (e.g., &AB), which is useful
 * for parsing and evaluation in logic solvers.
 */

#include <algorithm>
#include <stack>
#include <string>

#include "../common/parsetree_node.h"  ///< Contains helper functions like isOperator() and getPrecedence()

using namespace std;

/**
 * @brief Converts an infix propositional logic expression to prefix notation.
 * 
 * The conversion follows these steps:
 * 1. Reverse the infix expression.
 * 2. Swap the opening and closing parentheses.
 * 3. Convert the modified infix to postfix using a stack.
 * 4. Reverse the postfix to obtain the final prefix expression.
 * 
 * Supported operators depend on implementation of isOperator() and getPrecedence().
 * 
 * @param infix The input expression in infix notation (e.g., "A&(B|C)").
 * @return A string containing the expression in prefix notation (e.g., "&A|BC").
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
        if (isalnum(token)) {
            postfix += token;
        } else if (token == '(') {
            opStack.push(token);
        } else if (token == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.pop();  // Pop '('
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

    // Step 4: Reverse the postfix to get prefix.
    reverse(postfix.begin(), postfix.end());

    return postfix;
}
