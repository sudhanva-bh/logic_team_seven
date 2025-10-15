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
#include "tasks/task7_check_validity.cpp"

using namespace std;

int main() {
    vector<string> testCases = {
        "a",
        "~a",
        "a+b",
        "a*b",
        "a>b",
        "~a+b",
        "a*~b",
        "(a+b)*c",
        "~(a*b)+c",
        "a>(b+c)",
        "(a>b)>c",
        "((a>b)>(~c>d))+(d>(b+c))",  // Complex nested expression
        "a>a",
        "a>b"
        // "(((a > (b + (~c * d))) * ((~e + f) > (g * (h + ~i)))) + (((~(j * k))
        // > (l + m)) * ((n > (o * ~p)) + (q * (~r + (s > t))))) + (~((u + (v *
        // (~w + x))) > (y * (~z + (a > b))))))"
    };

    cout << "--- Parse Tree Height Test Cases ---\n\n";

    for (const string& s : testCases) {
        string prefix = infixToPrefix(s);
        Node* parseTree = prefixToParseTree(prefix);
        string reconstructedInfix = parseTreeToInfix(parseTree);
        int height = computeHeightOfParseTree(parseTree);
        Node* cnfParseTree = computeCnfFromParseTree(parseTree);
        // Node* cnfPrefix = computeCnf(prefix);

        cout << "Infix: " << s << '\n';
        // cout << "Prefix: " << prefix << '\n';
        // cout << "Reconstructed Infix: " << reconstructedInfix << '\n';
        // cout << "Height (edges): " << height << "\n";
        cout << "cnfParseTree: " << parseTreeToInfix(cnfParseTree) << "\n";
        cout << "cnfIsValid: " << isValid(cnfParseTree) << "\n";

        cout << string(40, '-') << "\n";
    }

    return 0;
}