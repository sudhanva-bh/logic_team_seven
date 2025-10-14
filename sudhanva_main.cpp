#include <algorithm>
#include <cctype>
#include <iostream>
#include <stack>
#include <vector>
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
 * @brief Helper function that recursively converts a parse tree to an infix expression.
 * @param currentNode Pointer to the current node being processed.
 * @param isRoot Flag indicating whether the current node is the root of the tree.
 * @return The corresponding infix expression for the current subtree.
 */
string parseTreeToInfixHelper(Node* currentNode, bool isRoot) {
    if (currentNode == nullptr) {
        return "";
    }

    char token = currentNode->data;

    if (token == '~') {
        return "(~" + parseTreeToInfixHelper(currentNode->right, false) + ")";
    } else if (isOperator(token)) {
        string left = parseTreeToInfixHelper(currentNode->left, false);
        string right = parseTreeToInfixHelper(currentNode->right, false);
        string result = left + token + right;

        return isRoot ? result : "(" + result + ")";
    } else {
        return string(1, token);
    }
}

/**
 * @brief Converts a parse tree into an infix expression string.
 *
 * This is the main wrapper function that starts the recursive process
 * from the root of the parse tree. The resulting expression omits unnecessary
 * outer parentheses.
 *
 * @param rootNode Pointer to the root node of the parse tree.
 * @return The reconstructed infix expression as a string.
 */
string parseTreeToInfix(Node* rootNode) {
    return parseTreeToInfixHelper(rootNode, true);
}

// --- Task 4: Computing the Height of a Parse Tree ---

/**
 * @brief Helper function that recursively computes the height (in edges)
 * of a parse tree.
 *
 * The height is defined as the number of edges on the longest path
 * from the current node down to a leaf.
 *
 * @param currentNode Pointer to the current node being processed.
 * @return int Height of the current subtree in edges.
 */
int computeHeightOfParseTreeHelper(Node* currentNode) {
    if (currentNode == nullptr) {
        return 0;
    }

    char token = currentNode->data;

    if (token == '~') {
        return 1 + computeHeightOfParseTreeHelper(currentNode->right);
    } else if (isOperator(token)) {
        int leftHeight = computeHeightOfParseTreeHelper(currentNode->left);
        int rightHeight = computeHeightOfParseTreeHelper(currentNode->right);
        return 1 + max(leftHeight, rightHeight);
    } else {
        return 0;
    }
}

/**
 * @brief Computes the height (in edges) of a parse tree rooted at the given node.
 *
 * This is the wrapper function that starts the recursive computation
 * of height from the root node.
 *
 * @param rootNode Pointer to the root of the parse tree.
 * @return int Height of the parse tree in terms of edges.
 */
int computeHeightOfParseTree(Node* rootNode) {
    return computeHeightOfParseTreeHelper(rootNode);
}

int main() {
    vector<string> testCases = {
        "",
        "a",
        "a+b",
        "a+b*c",
        "(a+b)*c",
        "~a",
        "~(a+b)",
        "a+(b*(c+d))",
        "((a+b)*(c-d))/e",
        "a>(b+(~c*d))",
        "((~a)+(b*(c+d)))",
        "(a+(b+(c+(d+e))))",
        "((((a+b)+c)+d)+e)",
        "(a*((b+c)*(d+(e*f))))"
    };

    cout << "--- Parse Tree Height Test Cases ---\n\n";

    for (const string& s : testCases) {
        string prefix = infixToPrefix(s);
        Node* parseTree = prefixToParseTree(prefix);
        string reconstructedInfix = parseTreeToInfix(parseTree);
        int height = computeHeightOfParseTree(parseTree);

        cout << "Infix: " << s << '\n';
        cout << "Prefix: " << prefix << '\n';
        cout << "Reconstructed Infix: " << reconstructedInfix << '\n';
        cout << "Height (edges): " << height << "\n";
        cout << string(40, '-') << "\n";
    }

    return 0;
}