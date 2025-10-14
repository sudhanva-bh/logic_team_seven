#include <algorithm>
#include <stack>
#include <string>

#include "../common/parsetree_node.h"

using namespace std;

// --- Task 1: Infix to Prefix Conversion ---

/**
 * @brief Converts a fully parenthesized infix propositional logic expression
 *        into its equivalent prefix expression.
 * 
 * This function implements the standard algorithm:
 * 1. Reverse the infix string.
 * 2. Swap '(' with ')' and vice versa.
 * 3. Convert the modified infix to postfix notation using a stack.
 * 4. Reverse the postfix expression to obtain the prefix expression.
 * 
 * Supported operators:
 * - Negation: '~'
 * - OR: '+'
 * - AND: '*'
 * - Implication: '>'
 * 
 * @param infix The fully parenthesized infix expression (e.g., "(a+b)*(c+d)").
 * @return std::string The equivalent prefix expression (e.g., "*+ab+cd").
 * 
 * @note The input must be fully parenthesized; otherwise, results may be incorrect.
 * @see isOperator(char c), getPrecedence(char op)
 * @cite [3] Standard algorithm for infix to prefix conversion in propositional logic.
 * 
 * @example
 * std::string infix = "(a+b)*(c+d)";
 * std::string prefix = infixToPrefix(infix);
 * // prefix now contains "*+ab+cd"
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
