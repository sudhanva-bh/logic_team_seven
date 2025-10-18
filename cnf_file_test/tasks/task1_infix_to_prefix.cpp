#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include <cctype>

#include "../common/parsetree_node.h"

/**
 * @brief Converts an infix logical expression string with multi-digit variables
 * (e.g., x1, x12) to a prefix token vector.
 *
 * This function implements the Shunting-yard algorithm variant for prefix
 * conversion. It first tokenizes the string into integers and then applies the algorithm.
 * Operands are expected to be in the format 'x' followed by one or more digits.
 *
 * @param infix The infix expression string (e.g., "(x1+x2)>x12").
 * @return A vector of integers representing the prefix expression.
 */
std::vector<int> infixToPrefix(const std::string& infix) {
    // Step 1 & 2: Tokenize and reverse the infix expression, swapping parentheses.
    std::vector<int> reversedTokens;
    for (int i = infix.length() - 1; i >= 0; --i) {
        char c = infix[i];

        if (isspace(c)) continue; // Ignore whitespace

        if (isdigit(c)) {
            std::string numStr;
            // Read the full number (which is reversed in the string)
            while (i >= 0 && isdigit(infix[i])) {
                numStr += infix[i];
                i--;
            }

            // After reading the number, the preceding character must be 'x'.
            if (i >= 0 && infix[i] == 'x') {
                std::reverse(numStr.begin(), numStr.end());
                int operandValue = std::stoi(numStr);
                reversedTokens.push_back(operandValue);
            } else {
                // Malformed token, e.g., a standalone number.
                // We backtrack 'i' to re-evaluate the non-'x' character.
                i += numStr.length();
            }
        } else if (c == ')') {
            reversedTokens.push_back(L_PAREN); // Note: swapped
        } else if (c == '(') {
            reversedTokens.push_back(R_PAREN); // Note: swapped
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

    // Step 3: Convert the tokenized and reversed infix to postfix (Shunting-yard).
    std::stack<int> opStack;
    std::vector<int> postfixTokens;

    for (int token : reversedTokens) {
        // If token is an operand (positive integer), add it to output.
        if (token > 0) {
            postfixTokens.push_back(token);
        }
        // If token is an opening parenthesis (swapped), push it onto the stack.
        else if (token == L_PAREN) {
            opStack.push(token);
        }
        // If token is a closing parenthesis (swapped), pop until L_PAREN is found.
        else if (token == R_PAREN) {
            while (!opStack.empty() && opStack.top() != L_PAREN) {
                postfixTokens.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) {
                opStack.pop(); // Pop the opening parenthesis
            }
        }
        // If token is a logical operator.
        else if (isOperator(token)) {
            // Pop operators with higher or equal precedence.
            while (!opStack.empty() && opStack.top() != L_PAREN &&
                   getPrecedence(token) < getPrecedence(opStack.top())) {
                postfixTokens.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
        }
    }

    // Pop any remaining operators from the stack.
    while (!opStack.empty()) {
        postfixTokens.push_back(opStack.top());
        opStack.pop();
    }

    // Step 4: Reverse the postfix token vector to get the final prefix expression.
    std::reverse(postfixTokens.begin(), postfixTokens.end());

    return postfixTokens;
}

/**
 * @brief Helper function to print the prefix vector in a readable format.
 */
void printPrefixVector(const std::vector<int>& prefix) {
    for (int token : prefix) {
        if (isOperator(token)) {
            switch (token) {
                case IMP: std::cout << "> "; break;
                case DIS: std::cout << "+ "; break;
                case CON: std::cout << "* "; break;
                case NEG: std::cout << "~ "; break;
            }
        } else {
            // Operands are positive integers
            std::cout << "x" << token << " ";
        }
    }
    std::cout << std::endl;
}
