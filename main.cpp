#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "common/parsetree_node.h"

// Include tasks
#include "common/generate_prepositional_logic_statements.cpp"
#include "tasks/task1_infix_to_prefix.cpp"
#include "tasks/task2_prefix_to_parse_tree.cpp"
#include "tasks/task3_parse_tree_to_infix.cpp"
#include "tasks/task4_compute_height__of_parse_tree.cpp"
#include "tasks/task5_evaluate_truth_value.cpp"
#include "tasks/task6_convert_to_cnf.cpp"
#include "tasks/task7_check_validity.cpp"
#include "tasks/task8_generate_truth_table.cpp"

using namespace std;
using namespace std::chrono;

int main() {
    srand(time(0));

    vector<string> testCases;
    map<char, bool> truth_values;
    bool doTruthTable = true;

    cout << "Do you want to enter a formula manually or generate randomly? "
            "(m/r): ";
    char choice = 'r';
    cin >> choice;
    cin.ignore();

    if (choice == 'r' || choice == 'R') {
        int numFormulas = 5, numVariables = 5, minDepth = 5, maxDepth = 5;
        double negProb = 0.5;

        cout << "Do you want to configure advanced options? (y/n): ";
        char advChoice;
        cin >> advChoice;
        cin.ignore();

        if (advChoice == 'y' || advChoice == 'Y') {
            cout << "Number of formulas (1-10): ";
            cin >> numFormulas;
            if (numFormulas < 1) numFormulas = 1;
            if (numFormulas > 100) {
                cout << "Capped number of formulas to 10.\n";
                numFormulas = 10;
            }

            cout << "Number of variables (1-10): ";
            cin >> numVariables;
            if (numVariables < 1) numVariables = 1;
            if (numVariables > 10) {
                cout << "Capped number of variables to 10.\n";
                numVariables = 26;
            }

            cout << "Minimum complexity (minDepth 1-6): ";
            cin >> minDepth;
            if (minDepth < 1) minDepth = 1;
            if (minDepth > 6) {
                cout << "Capped minDepth to 6.\n";
                minDepth = 7;
            }

            cout << "Maximum complexity (maxDepth 1-6): ";
            cin >> maxDepth;
            if (maxDepth < 1) maxDepth = 1;
            if (maxDepth > 6) {
                cout << "Capped maxDepth to 6.\n";
                maxDepth = 7;
            }

            if (minDepth > maxDepth) {
                cout << "Swapping minDepth and maxDepth for validity.\n";
                swap(minDepth, maxDepth);
            }

            cout << "Negation probability (0.0 - 1.0): ";
            cin >> negProb;
            if (negProb < 0.0) negProb = 0.0;
            if (negProb > 1.0) {
                cout << "Capped negation probability to 1.0.\n";
                negProb = 1.0;
            }
            cin.ignore();
        }

        cout << "Do you want to generate the truth table? (y/n): ";
        char ttChoice;
        cin >> ttChoice;
        cin.ignore();
        doTruthTable = (ttChoice == 'y' || ttChoice == 'Y');

        testCases = generateRandomFormulas(numFormulas, numVariables, minDepth,
                                           maxDepth, negProb);

        cout << "\nGenerated Formulas:\n";
        for (const auto &f : testCases) {
            cout << f << "\n\n";
        }
    } else {
        cout << "Enter the number of formulas you want to input: ";
        int n;
        cin >> n;
        cin.ignore();
        testCases.resize(n);
        for (int i = 0; i < n; ++i) {
            cout << "Formula " << (i + 1) << ": ";
            getline(cin, testCases[i]);
        }

        cout << "Do you want to generate the truth table? (y/n): ";
        char ttChoice;
        cin >> ttChoice;
        cin.ignore();
        doTruthTable = (ttChoice == 'y' || ttChoice == 'Y');

        set<char> atoms;
        for (const auto &f : testCases) {
            for (char c : f)
                if (isalpha(c)) atoms.insert(c);
        }

        cout << "\nEnter truth values for the following variables:\n";
        for (char var : atoms) {
            cout << var << " (T/F or 1/0): ";
            string val;
            cin >> val;
            truth_values[var] = (val == "T" || val == "t" || val == "1");
        }
        cin.ignore();
    }

    // Random mode truth values
    if (choice == 'r' || choice == 'R') {
        for (char c = 'a'; c <= 'z'; ++c) {
            truth_values[c] = rand() % 2;
        }
    }

    cout << "\n--- Running tasks and writing output to parse_output.txt ---\n";

    ofstream outFile("parse_output.txt");
    if (!outFile) {
        cerr << "Failed to open output file!" << endl;
        return 1;
    }

    streambuf *coutBuf = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());

    vector<long long> taskTimes(8, 0);
    vector<vector<long long>>
        taskTimesPerFormula;  // store per-formula task times

    for (const string &s : testCases) {
        vector<long long> thisFormulaTimes(8, 0);
        cout << "===========================================\n";
        cout << "Input Infix: " << s << "\n";
        cout << "===========================================\n";

        auto start = high_resolution_clock::now();
        string prefix = infixToPrefix(s);
        auto end = high_resolution_clock::now();
        thisFormulaTimes[0] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[0] += thisFormulaTimes[0];
        cout << "Task 1 (Infix to Prefix):      " << prefix << " ["
             << thisFormulaTimes[0] << " ns]\n";

        start = high_resolution_clock::now();
        Node *parseTree = prefixToParseTree(prefix);
        end = high_resolution_clock::now();
        thisFormulaTimes[1] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[1] += thisFormulaTimes[1];
        cout << "Task 2 (Prefix to ParseTree):  Done [" << thisFormulaTimes[1]
             << " ns]\n";

        start = high_resolution_clock::now();
        string reconstructedInfix = parseTreeToInfix(parseTree);
        end = high_resolution_clock::now();
        thisFormulaTimes[2] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[2] += thisFormulaTimes[2];
        cout << "Task 3 (ParseTree to Infix):   " << reconstructedInfix << " ["
             << thisFormulaTimes[2] << " ns]\n";

        start = high_resolution_clock::now();
        int height = computeHeightOfParseTree(parseTree);
        end = high_resolution_clock::now();
        thisFormulaTimes[3] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[3] += thisFormulaTimes[3];
        cout << "Task 4 (Compute Height):       " << height << " ["
             << thisFormulaTimes[3] << " ns]\n";

        cout << "\n--- Truth Values Used ---\n";
        set<char> atoms;
        for (char c : s)
            if (isalpha(c)) atoms.insert(c);

        for (char var : atoms) {
            cout << var << " = " << (truth_values[var] ? "True" : "False")
                 << "\n";
        }
        cout << "\n";

        start = high_resolution_clock::now();
        bool evaluatedTruthValue = evaluateTruthValue(parseTree, truth_values);
        end = high_resolution_clock::now();
        thisFormulaTimes[4] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[4] += thisFormulaTimes[4];
        cout << "Task 5 (Evaluate Truth):       "
             << (evaluatedTruthValue ? "True" : "False") << " ["
             << thisFormulaTimes[4] << " ns]\n";

        start = high_resolution_clock::now();
        Node *cnfParseTree = computeCnfFromParseTree(parseTree);
        end = high_resolution_clock::now();
        thisFormulaTimes[5] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[5] += thisFormulaTimes[5];
        cout << "Task 6 (Convert to CNF):       "
             << parseTreeToInfix(cnfParseTree) << " [" << thisFormulaTimes[5]
             << " ns]\n";

        start = high_resolution_clock::now();
        int validCount, invalidCount;
        bool cnfIsValid = isValid(cnfParseTree, validCount, invalidCount);
        end = high_resolution_clock::now();
        thisFormulaTimes[6] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[6] += thisFormulaTimes[6];
        cout << "Task 7 (Check Validity):       "
             << (cnfIsValid ? "Valid" : "Invalid") << " ["
             << thisFormulaTimes[6] << " µs]\n";
        cout << "Number of valid clauses:      " << validCount << "\n";
        cout << "Number of invalid clauses:    " << invalidCount << "\n";

        if (doTruthTable) {
            set<char> atoms;
            for (char c : s)
                if (isalpha(c)) atoms.insert(c);

            start = high_resolution_clock::now();
            cout << "Task 8 (Generate Truth Table):\n";
            generateTruthTable(cnfParseTree, atoms);
            end = high_resolution_clock::now();
            thisFormulaTimes[7] =
                duration_cast<nanoseconds>(end - start).count();
            taskTimes[7] += thisFormulaTimes[7];
            cout << " [Time: " << thisFormulaTimes[7] << " ns]\n";
        }

        destroyParseTree(parseTree);
        destroyParseTree(cnfParseTree);

        taskTimesPerFormula.push_back(thisFormulaTimes);
        cout << string(60, '-') << "\n\n";
    }

    // Restore console output
    cout.rdbuf(coutBuf);

    // Print averages to console
    cout << "--- Summary (Average Times per Task) ---\n";
    cout << "Task 1: " << taskTimes[0] / testCases.size() << " ns\n";
    cout << "Task 2: " << taskTimes[1] / testCases.size() << " ns\n";
    cout << "Task 3: " << taskTimes[2] / testCases.size() << " ns\n";
    cout << "Task 4: " << taskTimes[3] / testCases.size() << " ns\n";
    cout << "Task 5: " << taskTimes[4] / testCases.size() << " ns\n";
    cout << "Task 6: " << taskTimes[5] / testCases.size() << " ns\n";
    cout << "Task 7: " << taskTimes[6] / testCases.size() << " ns\n";
    if (doTruthTable)
        cout << "Task 8: " << taskTimes[7] / testCases.size() << " ns\n";

    cout << "Full task details written to parse_output.txt\n";

    // Append detailed time summary to output file
    outFile << "\n\n=== Performance Summary ===\n";
    for (size_t i = 0; i < testCases.size(); ++i) {
        outFile << "Formula " << (i + 1) << " timings:\n";
        outFile << "T1: " << taskTimesPerFormula[i][0] << " ns\n";
        outFile << "T2: " << taskTimesPerFormula[i][1] << " ns\n";
        outFile << "T3: " << taskTimesPerFormula[i][2] << " ns\n";
        outFile << "T4: " << taskTimesPerFormula[i][3] << " ns\n";
        outFile << "T5: " << taskTimesPerFormula[i][4] << " ns\n";
        outFile << "T6: " << taskTimesPerFormula[i][5] << " ns\n";
        outFile << "T7: " << taskTimesPerFormula[i][6] << " ns\n";
        if (doTruthTable)
            outFile << "T8: " << taskTimesPerFormula[i][7] << " ns\n";
        outFile << "---------------------------------------\n";
    }

    outFile << "\nAverage times:\n";
    outFile << "T1: " << taskTimes[0] / testCases.size() << " ns\n";
    outFile << "T2: " << taskTimes[1] / testCases.size() << " ns\n";
    outFile << "T3: " << taskTimes[2] / testCases.size() << " ns\n";
    outFile << "T4: " << taskTimes[3] / testCases.size() << " ns\n";
    outFile << "T5: " << taskTimes[4] / testCases.size() << " ns\n";
    outFile << "T6: " << taskTimes[5] / testCases.size() << " ns\n";
    outFile << "T7: " << taskTimes[6] / testCases.size() << " ns\n";
    if (doTruthTable)
        outFile << "T8: " << taskTimes[7] / testCases.size() << " ns\n";

    outFile.close();
    return 0;
}
