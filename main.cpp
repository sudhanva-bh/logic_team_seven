#include <iostream>
#include <vector>

#include "common/parsetree_node.h"

// Include tasks
#include "tasks/task1_infix_to_prefix.cpp"
#include "tasks/task2_prefix_to_parse_tree.cpp"
#include "tasks/task3_parse_tree_to_infix.cpp"
#include "tasks/task4_compute_height__of_parse_tree.cpp"
#include "tasks/task5_evaluate_truth_value.cpp"
#include "tasks/task6_convert_to_cnf.cpp"

using namespace std;

int main() {
    vector<string> testCases = {
        "a",             // Single variable
        "~a",            // Simple negation
        "a+b",           // Simple OR
        "a*b",           // Simple AND
        "a>b",           // Simple implication
        "~a+b",          // Negation + OR
        "a*~b",          // AND with negation
        "(a+b)*c",       // Mixed operators with parentheses
        "~(a*b)+c",      // Negation of AND with OR
        "a>(b+c)",       // Implication with OR on right
        "(a>b)>c",       // Implication on left with AND
        "(a>b)>(~c>d)"  // Complex nested expression
    };

    cout << "--- Parse Tree Height Test Cases ---\n\n";

    for (const string& s : testCases) {
        string prefix = infixToPrefix(s);
        Node* parseTree = prefixToParseTree(prefix);
        string reconstructedInfix = parseTreeToInfix(parseTree);
        int height = computeHeightOfParseTree(parseTree);
        Node* implFree = impl_free(parseTree);
        string implFreeString = parseTreeToInfix(implFree);
        Node* nnfForm = nnf(implFree);
        string nnfString = parseTreeToInfix(nnfForm);

        cout << "Infix: " << s << '\n';
        // cout << "Prefix: " << prefix << '\n';
        // cout << "Reconstructed Infix: " << reconstructedInfix << '\n';
        // cout << "Height (edges): " << height << "\n";
        cout << "ImplFree: " << implFreeString << "\n";
        cout << "NNF: " << nnfString << "\n";
        cout << string(40, '-') << "\n";
    }

    return 0;
}