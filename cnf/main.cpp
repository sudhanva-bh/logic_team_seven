#include <chrono>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "common/parsetree_node.h"

// Declare functions from the tasks
#include "tasks/task0_convert_the_file_into_infix.cpp"
#include "tasks/task1_infix_to_prefix.cpp"
#include "tasks/task2_prefix_to_parse_tree.cpp"
#include "tasks/task3_parse_tree_to_infix.cpp"
#include "tasks/task4_compute_height__of_parse_tree.cpp"
#include "tasks/task5_evaluate_truth_value.cpp"
#include "tasks/task7_check_validity.cpp"

using namespace std;
using namespace std::chrono;

int main() {
    string filepath;
    cout << "Enter the path to the .cnf file: ";
    getline(cin, filepath);

    // Read CNF file and convert to infix
    string cnfContents = readCNFFile(filepath);
    string infix = cnfToInfix(cnfContents);

    vector<string> testCases = {infix};
    map<char, bool> truth_values;
    set<char> atoms;
    for (char c : infix)
        if (isalpha(c)) atoms.insert(c);

    // Assign random truth values
    for (char var : atoms) truth_values[var] = rand() % 2;

    // Open output file
    ofstream outFile("parse_output.txt");
    if (!outFile) {
        cerr << "Failed to open output file!" << endl;
        return 1;
    }
    streambuf* coutBuf = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());

    vector<long long> taskTimes(5, 0);
    vector<vector<long long>> taskTimesPerFormula;

    for (const string& s : testCases) {
        vector<long long> thisFormulaTimes(5, 0);
        cout << "===========================================\n";
        cout << "Input Infix: " << s << "\n";
        cout << "===========================================\n";

        // Task 1: CNF to Infix (already done, just log time)
        auto start = high_resolution_clock::now();
        // already done
        auto end = high_resolution_clock::now();
        thisFormulaTimes[0] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[0] += thisFormulaTimes[0];
        cout << "Task 1 (CNF to Infix): " << s << " [" << thisFormulaTimes[0]
             << " ns]\n";

        // Task 2: Infix to Prefix
        start = high_resolution_clock::now();
        string prefix = infixToPrefix(s);
        end = high_resolution_clock::now();
        thisFormulaTimes[1] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[1] += thisFormulaTimes[1];
        cout << "Task 2 (Infix to Prefix): " << prefix << " ["
             << thisFormulaTimes[1] << " ns]\n";

        // Task 3: Prefix to ParseTree
        start = high_resolution_clock::now();
        Node* parseTree = prefixToParseTree(prefix);
        end = high_resolution_clock::now();
        thisFormulaTimes[2] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[2] += thisFormulaTimes[2];
        cout << "Task 3 (Prefix to ParseTree): Done [" << thisFormulaTimes[2]
             << " ns]\n";

        // Task 4: ParseTree to Infix
        start = high_resolution_clock::now();
        string reconstructedInfix = parseTreeToInfix(parseTree);
        end = high_resolution_clock::now();
        thisFormulaTimes[3] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[3] += thisFormulaTimes[3];
        cout << "Task 4 (ParseTree to Infix): " << reconstructedInfix << " ["
             << thisFormulaTimes[3] << " ns]\n";

        // Task 5: Check Validity
        start = high_resolution_clock::now();
        int validCount, invalidCount;
        bool cnfIsValid = isValid(cnfContents, validCount, invalidCount);
        end = high_resolution_clock::now();
        thisFormulaTimes[4] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[4] += thisFormulaTimes[4];
        cout << "Task 5 (Check Validity): "
             << (cnfIsValid ? "Valid" : "Invalid") << " ["
             << thisFormulaTimes[4] << " ns]\n";
        cout << "Number of valid clauses: " << validCount << "\n";
        cout << "Number of invalid clauses: " << invalidCount << "\n";

        destroyParseTree(parseTree);

        taskTimesPerFormula.push_back(thisFormulaTimes);
        cout << string(60, '-') << "\n\n";
    }

    // Restore console output
    cout.rdbuf(coutBuf);

    // Summary
    cout << "--- Summary (Times per Task) ---\n";
    for (int i = 0; i < 5; ++i)
        cout << "Task " << (i + 1) << ": " << taskTimes[i] / testCases.size()
             << " ns\n";

    cout << "Full task details written to parse_output.txt\n";

    // Write per-formula timings to file
    outFile << "\n\n=== Performance Summary ===\n";
    for (size_t i = 0; i < testCases.size(); ++i) {
        outFile << "Formula " << (i + 1) << " timings:\n";
        for (int t = 0; t < 5; ++t)
            outFile << "T" << (t + 1) << ": " << taskTimesPerFormula[i][t]
                    << " ns\n";
        outFile << "---------------------------------------\n";
    }

    outFile.close();
    return 0;
}
