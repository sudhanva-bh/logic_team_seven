/**
 * @file task1_infix_to_prefix.cpp
 * @brief Converts an infix logical expression to prefix (Polish) notation.
 *
 * @details This file defines a function that transforms a propositional logic
 * expression from its standard infix form (e.g., A * B) to its equivalent
 * prefix form (e.g., *AB). This notation is unambiguous and ideal for
 * parsing and evaluation in logic solvers or for building expression trees.
 */

#include <algorithm> // For std::reverse
#include <stack>     // For std::stack
#include <string>    // For std::string

// This file is assumed to provide the isOperator() and getPrecedence() helpers.
#include "../common/parsetree_node.h" ///< Contains helper functions like isOperator() and getPrecedence()

using namespace std;

/**
 * @brief Converts an infix propositional logic expression to prefix notation.
 *
 * @details
 * This function implements the standard algorithm for infix-to-prefix conversion
 * by adapting the Shunting-yard (infix-to-postfix) algorithm.
 *
 * The algorithm proceeds in four steps:
 * 1.  **Reverse** the original infix string.
 * 2.  **Swap** all parentheses. '(' becomes ')' and ')' becomes '('. This
 * adapts the expression for the upcoming postfix conversion, as the
 * reversal also reversed operator associativity.
 * 3.  **Convert** the modified (reversed and swapped-paren) string to
 * *postfix* notation using a stack-based algorithm.
 * 4.  **Reverse** the resulting postfix string. This final reversal
 * yields the correct prefix notation.
 *
 * @param infix The input expression in infix notation (e.g., "A*(B|C)").
 * @return A string containing the expression in prefix notation (e.g., "*A|BC").
 */
string infixToPrefix(string infix)
{
    // Step 1: Reverse the infix expression.
    reverse(infix.begin(), infix.end());

    // Step 2: Swap opening and closing parentheses.
    // This is crucial for correctly handling operator precedence
    // after the string has been reversed.
    for (size_t i = 0; i < infix.length(); ++i)
    {
        if (infix[i] == '(')
        {
            infix[i] = ')';
        }
        else if (infix[i] == ')')
        {
            infix[i] = '(';
        }
    }

    // Step 3: Convert the modified infix string to postfix.
    stack<char> opStack;
    string postfix;

    for (char token : infix)
    {
        // If the token is an operand (letter or number), add it to the output.
        if (isalnum(token))
        {
            postfix += token;
        }
        // If it's an opening parenthesis, push it onto the operator stack.
        else if (token == '(')
        {
            opStack.push(token);
        }
        // If it's a closing parenthesis, pop operators from the stack
        // to the output until the matching opening parenthesis is found.
        else if (token == ')')
        {
            while (!opStack.empty() && opStack.top() != '(')
            {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.pop(); // Pop and discard the opening parenthesis '('.
        }
        // If the token is an operator...
        else if (isOperator(token))
        {
            // Pop operators from the stack that have higher precedence
            // than the current token.
            while (!opStack.empty() && opStack.top() != '(' &&
                   getPrecedence(token) < getPrecedence(opStack.top()))
            {
                postfix += opStack.top();
                opStack.pop();
            }
            // Push the current operator onto the stack.
            opStack.push(token);
        }
    }

    // After processing all tokens, pop any remaining operators from the stack.
    while (!opStack.empty())
    {
        postfix += opStack.top();
        opStack.pop();
    }

    // Step 4: Reverse the postfix string to get the final prefix.
    reverse(postfix.begin(), postfix.end());

    return postfix;
}