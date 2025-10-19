#include <iostream>
#include <string>

#include "common/parsetree_node.h"

// Declare functions from the tasks
using namespace std;

#include "tasks/task0_convert_the_file_into_infix.cpp"
#include "tasks/task1_infix_to_prefix.cpp"
#include "tasks/task2_prefix_to_parse_tree.cpp"
#include "tasks/task3_parse_tree_to_infix.cpp"
#include "tasks/task4_compute_height__of_parse_tree.cpp"
#include "tasks/task5_evaluate_truth_value.cpp"
// #include "tasks/task6_convert_to_cnf.cpp"
#include "tasks/task7_check_validity.cpp"
// #include "tasks/task8_generate_truth_table.cpp"

int main() {
    string filepath;
    cout << "Enter the path to the .cnf file: ";
    getline(cin, filepath);

    try {
        // Step 0 → 1: CNF file to infix
        string cnfContents = readCNFFile(filepath);
        string infix = cnfToInfix(cnfContents);
        cout << "\nStep 1 - Infix from CNF:\n" << infix << endl;

        // Step 1 → 2: Infix to prefix
        string prefix = infixToPrefix(infix);
        cout << "\nStep 2 - Prefix expression:\n" << prefix << endl;

        // Step 2 → 3: Prefix to parse tree
        Node* root = prefixToParseTree(prefix);

        // Step 3 → 4: Parse tree back to infix
        string finalInfix = parseTreeToInfix(root);
        cout << "\nStep 3 - Infix from parse tree:\n" << finalInfix << endl;

        int height = computeHeightOfParseTree(root);
        cout << "\nStep 4 - Height of parse tree:\n" << height << endl;

        int validCount, invalidCount;
        bool isValidCnf = isValid(cnfContents, validCount, invalidCount);
        cout << "\nStep 5 - isValid:\n" << isValidCnf << endl;
        cout << "\nValid:" << validCount << "  Invalid:" << invalidCount
             << endl;

        // Clean up
        destroyParseTree(root);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
