#include <string>

#include "../common/parsetree_node.h"

using namespace std;

namespace {
void buildParseTreeToPrefixRecursive(Node* currentNode, std::string& result) {
    if (currentNode == nullptr) {
        return;
    }

    // Append a space if the string is not empty
    if (!result.empty()) {
        result += " ";
    }

    int token = currentNode->data;
    if (token > 0) {
        result += "x" + std::to_string(token);
    } else {
        switch (token) {
            case IMP: result += ">"; break;
            case DIS: result += "+"; break;
            case CON: result += "*"; break;
            case NEG: result += "~"; break;
        }
    }

    // Recurse on children
    buildParseTreeToPrefixRecursive(currentNode->left, result);
    buildParseTreeToPrefixRecursive(currentNode->right, result);
}
} // namespace

/**
 * @brief Converts a parse tree back into an infix notation string.
 * @param rootNode The root of the parse tree.
 * @return std::string The infix expression.
 */
std::string parseTreeToInfix(Node* rootNode) {
    return buildParseTreeToInfixRecursive(rootNode, true);
}