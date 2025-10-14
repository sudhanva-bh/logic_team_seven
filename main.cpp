#include <iostream>
#include <vector>

#include "common/parsetree_node.h"

// Include tasks
#include "tasks/task1_infix_to_prefix.cpp"
#include "tasks/task2_prefix_to_parse_tree.cpp"
#include "tasks/task3_parse_tree_to_infix.cpp"
#include "tasks/task4_compute_height__of_parse_tree.cpp"

using namespace std;

int main() {
    vector<string> testCases = {"",
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
                                "(a*((b+c)*(d+(e*f))))"};

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