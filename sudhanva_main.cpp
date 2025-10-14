#include <algorithm>
#include <cctype>
#include <iostream>
#include <stack>
using namespace std;

struct Node {
    char data;
    Node* left;
    Node* right;

    Node(char val) : data(val), left(nullptr), right(nullptr) {};
};

bool isOperator(char c) { return c == '~' || c == '+' || c == '*' || c == '>'; }

int getPrecedence(char op) {
    switch (op) {
        case '>':
            return 1;
        case '+':
            return 2;
        case '*':
            return 3;
        case '~':
            return 4;
        default:
            return 0;  // For operands and parentheses
    }
}

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

// --- Task 2: Prefix to Parse Tree ---

/**
 * @brief Recursively builds a parse tree from a prefix expression string.
 * @param prefix The prefix expression.
 * @param index The current position in the string (passed by reference).
 * @return A pointer to the root of the constructed subtree.
 */
Node* buildTreeFromPrefixRecursive(const string& prefix, int& index) {
    if (index == prefix.length()) {
        return nullptr;
    }

    char token = prefix[index++];
    Node* node = new Node(token);

    if (isOperator(token)) {
        if (token == '~') {
            node->right = buildTreeFromPrefixRecursive(prefix, index);
        } else {
            node->left = buildTreeFromPrefixRecursive(prefix, index);
            node->right = buildTreeFromPrefixRecursive(prefix, index);
        }
    }

    return node;
}

/**
 * @brief Converts a prefix expression into a rooted binary parse tree.
 * @param prefix The prefix expression string.
 * @return A pointer to the root of the newly created parse tree.
 */
Node* prefixToParseTree(const std::string& prefix) {
    int index = 0;
    return buildTreeFromPrefixRecursive(prefix, index);
}

// --- Task 3: Parse Tree to Infix Expression ---

/**
 * @brief Recursively converts a parse tree back into an infix expression
 * string.
 *
 * This function performs an in-order traversal of the given parse tree.
 * It places parentheses around each binary operation to preserve correct
 * operator precedence, but omits the outermost parentheses for the root node.
 *
 * @param currentNode Pointer to the current node being processed.
 * @param isRoot Flag indicating whether the current node is the root of the
 * tree. (Used to skip outermost parentheses.)
 * @return The resulting infix expression as a string.
 */
string parseTreeToInfix(Node* currentNode, bool isRoot = true) {
    if (currentNode == nullptr) {
        return "";
    }

    char token = currentNode->data;

    if (token == '~') {
        return "(~" + parseTreeToInfix(currentNode->right, false) + ")";
    } else if (isOperator(token)) {
        string left = parseTreeToInfix(currentNode->left, false);
        string right = parseTreeToInfix(currentNode->right, false);
        string result = left + token + right;

        return isRoot ? result : "(" + result + ")";
    } else {
        return string(1, token);
    }
}

int main() {
    string s = "a>(b+(~c*d))";
    string prefix = infixToPrefix(s);
    Node* parseTree = prefixToParseTree(prefix);
    string parseTreeToInf = parseTreeToInfix(parseTree);
    cout << "\nprefix: " << prefix;
    cout << "\nparseTreeToInf: " << parseTreeToInf;
}