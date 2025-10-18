// #include <chrono>
// #include <fstream>
// #include <iostream>
// #include <map>
// #include <string>
// #include <vector>

// #include "common/parsetree_node.h"

// // Include tasks
// #include "tasks/task1_infix_to_prefix.cpp"
// #include "tasks/task2_prefix_to_parse_tree.cpp"
// #include "tasks/task3_parse_tree_to_infix.cpp"
// #include "tasks/task4_compute_height__of_parse_tree.cpp"
// #include "tasks/task5_evaluate_truth_value.cpp"
// #include "tasks/task6_convert_to_cnf.cpp"
// #include "tasks/task7_check_validity.cpp"
// #include "tasks/task8_generate_truth_table.cpp"

// using namespace std;
// using namespace std::chrono;

// int main() {
//     vector<string> testCases = {
//         "((a>b)>((~c+d)*(e>(~f+g))))*((~(h>i))+(j*(~a>c)))*(((d+~e)>(f*g))>((~"
//         "b+h)*(~(i+j))))*(((~g>(~h>i))+(a*(~b+~c)))>((~d+e)*(f>~j)))+((((~a+b)*"
//         "(~c+d))>((~e+f)*(g>~h)))+((i>~j)*(~(a+b)>~(c+d))))",
//         "(((~a+(b*c))>(d*~(e>f)))*(~(g*h)+((j>k)*~a)))>((((b+~d)*(e*f))>(g+(h*~"
//         "j)))*((k>a)+(c*~b)))"};

//     map<char, bool> truth_values = {
//         {'a', true},  {'b', false}, {'c', true},  {'d', true},  {'e', false},
//         {'f', true},  {'g', false}, {'h', true},  {'i', false}, {'j', true},
//         {'k', false}, {'l', true},  {'m', true},  {'n', false}, {'o', true},
//         {'p', false}, {'q', true},  {'r', false}, {'s', true},  {'t', true},
//         {'u', false}, {'v', true},  {'w', false}, {'x', true},  {'y', false},
//         {'z', true}};

//     cout << "--- Parse Tree Test Suite ---\n\n";

//     // Open output file
//     ofstream outFile("parse_output.txt");
//     if (!outFile) {
//         cerr << "Failed to open output file!" << endl;
//         return 1;
//     }

//     // Redirect cout to file
//     streambuf* coutBuf = cout.rdbuf();
//     cout.rdbuf(outFile.rdbuf());

//     vector<long long> taskTimes(8, 0);  // accumulate time for each task

//     for (const string& s : testCases) {
//         cout << "===========================================\n";
//         cout << "Input Infix: " << s << "\n";
//         cout << "===========================================\n";

//         auto start = high_resolution_clock::now();
//         string prefix = infixToPrefix(s);
//         auto end = high_resolution_clock::now();
//         taskTimes[0] += duration_cast<nanoseconds>(end - start).count();
//         cout << "Task 1 (Infix to Prefix):      " << prefix << "\n";

//         start = high_resolution_clock::now();
//         Node* parseTree = prefixToParseTree(prefix);
//         end = high_resolution_clock::now();
//         taskTimes[1] += duration_cast<nanoseconds>(end - start).count();
//         cout << "Task 2 (Prefix to ParseTree):  Done\n";

//         start = high_resolution_clock::now();
//         string reconstructedInfix = parseTreeToInfix(parseTree);
//         end = high_resolution_clock::now();
//         taskTimes[2] += duration_cast<nanoseconds>(end - start).count();
//         cout << "Task 3 (ParseTree to Infix):   " << reconstructedInfix << "\n";

//         start = high_resolution_clock::now();
//         int height = computeHeightOfParseTree(parseTree);
//         end = high_resolution_clock::now();
//         taskTimes[3] += duration_cast<nanoseconds>(end - start).count();
//         cout << "Task 4 (Compute Height):      " << height << "\n";

//         start = high_resolution_clock::now();
//         bool evaluatedTruthValue = evaluateTruthValue(parseTree, truth_values);
//         end = high_resolution_clock::now();
//         taskTimes[4] += duration_cast<nanoseconds>(end - start).count();
//         cout << "Task 5 (Evaluate Truth):      "
//              << (evaluatedTruthValue ? "True" : "False") << "\n";

//         start = high_resolution_clock::now();
//         Node* cnfParseTree = computeCnfFromParseTree(parseTree);
//         end = high_resolution_clock::now();
//         taskTimes[5] += duration_cast<nanoseconds>(end - start).count();
//         cout << "Task 6 (Convert to CNF):      "
//              << parseTreeToInfix(cnfParseTree) << "\n";

//         start = high_resolution_clock::now();
//         int validCount, invalidCount;
//         bool cnfIsValid = isValid(cnfParseTree, validCount, invalidCount);
//         end = high_resolution_clock::now();
//         taskTimes[6] += duration_cast<microseconds>(end - start).count();
//         cout << "Task 7 (Check Validity):      "
//              << (cnfIsValid ? "Valid" : "Invalid") << "\n";

//         start = high_resolution_clock::now();
//         cout << "Task 8 (Generate Truth Table):\n";
//         generateTruthTable(cnfParseTree);
//         end = high_resolution_clock::now();
//         taskTimes[7] += duration_cast<milliseconds>(end - start).count();

//         destroyParseTree(parseTree);
//         destroyParseTree(cnfParseTree);

//         cout << string(60, '-') << "\n\n";
//     }

//     // Restore cout to console
//     cout.rdbuf(coutBuf);

//     // Display summary/statistics
//     cout << "--- Summary (Average Times per Task) ---\n";
//     cout << "Task 1: " << taskTimes[0] / testCases.size() << " ns\n";
//     cout << "Task 2: " << taskTimes[1] / testCases.size() << " ns\n";
//     cout << "Task 3: " << taskTimes[2] / testCases.size() << " ns\n";
//     cout << "Task 4: " << taskTimes[3] / testCases.size() << " ns\n";
//     cout << "Task 5: " << taskTimes[4] / testCases.size() << " ns\n";
//     cout << "Task 6: " << taskTimes[5] / testCases.size() << " ns\n";
//     cout << "Task 7: " << taskTimes[6] / testCases.size() << " mis\n";
//     cout << "Task 8: " << taskTimes[7] / testCases.size() << " ms\n";

//     outFile.close();
//     cout << "Full task details written to parse_output.txt\n";

//     return 0;
// }


#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Atoms available
const string atoms = "abcdefghijklmnopqrstuvwxyz";

// Generate a random atom
char randomAtom() {
    return atoms[rand() % atoms.size()];
}

// Randomly choose a binary operator: +, *, >
char randomBinaryOp() {
    const string ops = "+*>"; // OR, AND, IMPLIES
    return ops[rand() % ops.size()];
}

// Randomly decide if we use NOT
bool randomNegation() {
    return rand() % 2 == 0;
}

// Recursive function to generate a well-formed formula
string generateFormula(int depth, int maxDepth) {
    // Base case: small depth, just return a literal (maybe negated)
    if (depth >= maxDepth || (depth > 0 && rand() % 2 == 0)) {
        string lit = "";
        if (randomNegation()) lit += "~";
        lit += randomAtom();
        return lit;
    }

    // Otherwise, generate binary expression
    string left = generateFormula(depth + 1, maxDepth);
    string right = generateFormula(depth + 1, maxDepth);
    char op = randomBinaryOp();
    return "(" + left + op + right + ")";
}

int main() {
    srand(time(0)); // seed randomness

    int numFormulas = 5;   // generate 5 formulas
    int maxDepth = 5;      // maximum recursion depth

    for (int i = 0; i < numFormulas; ++i) {
        string formula = generateFormula(0, maxDepth);
        cout << formula << "\n\n";
    }

    return 0;
}
