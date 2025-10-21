#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <conio.h>  // Required for _getch()
#include <stdlib.h> // Required for system("cls")

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

/**
 * @brief Displays an interactive menu and returns the user's selected index.
 * @param prompt The question to ask the user.
 * @param options A vector of strings representing the choices.
 * @return The 0-based index of the selected option.
 */
int getUserChoiceMenu(const string& prompt, const vector<string>& options) {
    int selected = 0;
    int numOptions = options.size();

    while (true) {
        system("cls"); // Clear the console
        cout << prompt << "\n";
        cout << "Use UP/DOWN keys to navigate and ENTER to select.\n";
        cout << "--------------------------------------------------\n\n";

        for (int i = 0; i < numOptions; ++i) {
            if (i == selected) {
                // Highlight the selected option
                cout << " > " << options[i] << " <\n";
            } else {
                cout << "   " << options[i] << " \n";
            }
        }

        int ch = _getch(); // Wait for a key press
        if (ch == 224) { // Special key (arrow keys)
            ch = _getch(); // Get the actual key code
            if (ch == 72) { // Up Arrow
                selected = (selected - 1 + numOptions) % numOptions;
            } else if (ch == 80) { // Down Arrow
                selected = (selected + 1) % numOptions;
            }
        } else if (ch == 13) { // Enter key
            system("cls"); // Clean up the console
            return selected;
        }
    }
}

int runGeneral(string filePath) {
    srand(time(0));

    vector<string> testCases;
    map<char, bool> truth_values;
    bool doTruthTable = false;
    bool doEvaluateSpecific = false;

    // ✅ If filePath is provided, load testCases from the text file
    if (!filePath.empty()) {
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "Error: could not open file " << filePath << endl;
            return 1;
        }

        string line;
        while (getline(file, line)) {
            if (!line.empty()) testCases.push_back(line);
        }
        file.close();

        if (testCases.empty()) {
            cerr << "File is empty or invalid format." << endl;
            return 1;
        }

        cout << "Loaded " << testCases.size() << " formulas from " << filePath
             << ".\n";

        // Detect all variables
        set<char> atoms;
        for (const auto& f : testCases) {
            for (char c : f)
                if (isalpha(c)) atoms.insert(c);
        }

        // --- Replaced 'y/n' with menu ---
        int ttChoice = getUserChoiceMenu("Do you want to generate the truth table?", 
                                       {"Yes", "No"});
        doTruthTable = (ttChoice == 0); // 0 is "Yes"

        // --- Replaced 'y/n' with menu ---
        int evalChoice = getUserChoiceMenu("Do you want to evaluate the formulae for a specific set of truth values?", 
                                         {"Yes", "No"});
        doEvaluateSpecific = (evalChoice == 0); // 0 is "Yes"

        if (doEvaluateSpecific) {
            cout << "\nEnter truth values for the following variables:\n";
            for (char var : atoms) {
                cout << var << " (T/F or 1/0): ";
                string val;
                cin >> val;
                truth_values[var] = (val == "T" || val == "t" || val == "1");
            }
            cin.ignore();
        } else {
            // Assign random values if not provided
            for (char var : atoms) truth_values[var] = rand() % 2;
        }
    } else {
        // 🔹 Original logic: manual or random generation
        
        // --- Replaced 'm/r' with menu ---
        int choice = getUserChoiceMenu("How do you want to provide formulas?", 
                                     {"Generate randomly", "Enter formulas manually"});

        if (choice == 0) { // 0 is "Generate randomly"
            // Random generation
            int numFormulas = 5, numVariables = 5, minDepth = 5, maxDepth = 5;
            double negProb = 0.5;

            // --- Replaced 'y/n' with menu ---
            int advChoice = getUserChoiceMenu("Do you want to configure advanced options?", 
                                            {"No", "Yes"});

            if (advChoice == 1) { // 1 is "Yes"
                cout << "Number of formulas (1-100): ";
                cin >> numFormulas;
                if (numFormulas < 1) numFormulas = 1;
                if (numFormulas > 100) numFormulas = 100;

                cout << "Number of variables (1-10): ";
                cin >> numVariables;
                if (numVariables < 1) numVariables = 1;
                if (numVariables > 10) numVariables = 10;

                cout << "Minimum complexity (1-6): ";
                cin >> minDepth;
                cout << "Maximum complexity (1-6): ";
                cin >> maxDepth;
                if (minDepth > maxDepth) swap(minDepth, maxDepth);

                cout << "negNode probability (0.0 - 1.0): ";
                cin >> negProb;
                if (negProb < 0.0) negProb = 0.0;
                if (negProb > 1.0) negProb = 1.0;
                cin.ignore();
            }

            // --- Replaced 'y/n' with menu ---
            int ttChoice = getUserChoiceMenu("Do you want to generate the truth table?", 
                                           {"Yes", "No"});
            doTruthTable = (ttChoice == 0); // 0 is "Yes"

            testCases = generateRandomFormulas(numFormulas, numVariables,
                                               minDepth, maxDepth, negProb);

            for (char c = 'a'; c <= 'z'; ++c) truth_values[c] = rand() % 2;

            cout << "\nGenerated Formulas:\n";
            for (const auto& f : testCases) cout << f << "\n\n";

            cout << "Press any key to continue...";
            _getch();

        } else { // 1 is "Enter formulas manually"
            // Manual input
            cout << "Enter the number of formulas you want to input: ";
            int n;
            cin >> n;
            cin.ignore();
            testCases.resize(n);
            for (int i = 0; i < n; ++i) {
                cout << "Formula " << (i + 1) << ": ";
                getline(cin, testCases[i]);
            }

            set<char> atoms;
            for (const auto& f : testCases)
                for (char c : f)
                    if (isalpha(c)) atoms.insert(c);

            // --- Replaced 'y/n' with menu ---
            int ttChoice = getUserChoiceMenu("Do you want to generate the truth table?", 
                                           {"Yes", "No"});
            doTruthTable = (ttChoice == 0); // 0 is "Yes"

            // --- Replaced 'y/n' with menu ---
            int evalChoice = getUserChoiceMenu("Do you want to evaluate the formulae for a specific set of truth values?", 
                                             {"Yes", "No"});
            doEvaluateSpecific = (evalChoice == 0); // 0 is "Yes"

            if (doEvaluateSpecific) {
                cout << "\nEnter truth values for the following variables:\n";
                for (char var : atoms) {
                    cout << var << " (T/F or 1/0): ";
                    string val;
                    cin >> val;
                    truth_values[var] =
                        (val == "T" || val == "t" || val == "1");
                }
                cin.ignore();
            } else {
                for (char var : atoms) truth_values[var] = rand() % 2;
            }
        }
    }

    // Open output file
    ofstream outFile("parse_output.txt");
    if (!outFile) {
        cerr << "Failed to open output file!" << endl;
        return 1;
    }
    streambuf* coutBuf = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());

    vector<long long> taskTimes(8, 0);
    vector<vector<long long>> taskTimesPerFormula;

    for (const string& s : testCases) {
        vector<long long> thisFormulaTimes(8, 0);
        cout << "===========================================\n";
        cout << "Input Infix: " << s << "\n";
        cout << "===========================================\n";

        // Task 1: Infix to Prefix
        auto start = high_resolution_clock::now();
        string prefix = infixToPrefix(s);
        auto end = high_resolution_clock::now();
        thisFormulaTimes[0] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[0] += thisFormulaTimes[0];
        cout << "Task 1 (Infix to Prefix): " << prefix << " ["
             << thisFormulaTimes[0] << " ns]\n";

        // Task 2: Prefix to ParseTree
        start = high_resolution_clock::now();
        Node* parseTree = prefixToParseTree(prefix);
        end = high_resolution_clock::now();
        thisFormulaTimes[1] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[1] += thisFormulaTimes[1];
        cout << "Task 2 (Prefix to ParseTree): Done [" << thisFormulaTimes[1]
             << " ns]\n";

        // Task 3: ParseTree to Infix
        start = high_resolution_clock::now();
        string reconstructedInfix = parseTreeToInfix(parseTree);
        end = high_resolution_clock::now();
        thisFormulaTimes[2] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[2] += thisFormulaTimes[2];
        cout << "Task 3 (ParseTree to Infix): " << reconstructedInfix << " ["
             << thisFormulaTimes[2] << " ns]\n";

        // Task 4: Compute Height
        start = high_resolution_clock::now();
        int height = computeHeightOfParseTree(parseTree);
        end = high_resolution_clock::now();
        thisFormulaTimes[3] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[3] += thisFormulaTimes[3];
        cout << "Task 4 (Compute Height): " << height << " ["
             << thisFormulaTimes[3] << " ns]\n";

        // If user wants to evaluate for specific truth values
        if (doEvaluateSpecific) {
            cout << "\n--- Truth Values Used ---\n";
            set<char> atoms;
            for (char c : s)
                if (isalpha(c)) atoms.insert(c);
            for (char var : atoms)
                cout << var << " = " << (truth_values[var] ? "True" : "False")
                     << "\n";
            start = high_resolution_clock::now();
            bool evaluatedTruthValue =
                evaluateTruthValue(parseTree, truth_values);
            end = high_resolution_clock::now();
            thisFormulaTimes[4] =
                duration_cast<nanoseconds>(end - start).count();
            taskTimes[4] += thisFormulaTimes[4];
            cout << "Task 5 (Evaluate Truth): "
                 << (evaluatedTruthValue ? "True" : "False") << " ["
                 << thisFormulaTimes[4] << " ns]\n";
        }

        // Task 6: Convert to CNF
        start = high_resolution_clock::now();
        Node* cnfParseTree = computeCnfFromParseTree(parseTree);
        end = high_resolution_clock::now();
        thisFormulaTimes[5] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[5] += thisFormulaTimes[5];
        cout << "Task 6 (Convert to CNF): " << parseTreeToInfix(cnfParseTree)
             << " [" << thisFormulaTimes[5] << " ns]\n";

        // Task 7: Check Validity
        start = high_resolution_clock::now();
        int validCount, invalidCount;
        bool cnfIsValid = isValid(cnfParseTree, validCount, invalidCount);
        end = high_resolution_clock::now();
        thisFormulaTimes[6] = duration_cast<nanoseconds>(end - start).count();
        taskTimes[6] += thisFormulaTimes[6];
        cout << "Task 7 (Check Validity): "
             << (cnfIsValid ? "Valid" : "Invalid") << " ["
             << thisFormulaTimes[6] << " ns]\n";
        cout << "Number of valid clauses: " << validCount << "\n";
        cout << "Number of invalid clauses: " << invalidCount << "\n";

        // Task 8: Generate Truth Table
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

    // Summary
    cout << "--- Summary (Average Times per Task) ---\n";
    cout << "Task 1: " << taskTimes[0] / testCases.size() << " ns\n";
    cout << "Task 2: " << taskTimes[1] / testCases.size() << " ns\n";
    cout << "Task 3: " << taskTimes[2] / testCases.size() << " ns\n";
    cout << "Task 4: " << taskTimes[3] / testCases.size() << " ns\n";
    if (doEvaluateSpecific)
        cout << "Task 5: " << taskTimes[4] / testCases.size() << " ns\n";
    cout << "Task 6: " << taskTimes[5] / testCases.size() << " ns\n";
    cout << "Task 7: " << taskTimes[6] / testCases.size() << " ns\n";
    if (doTruthTable)
        cout << "Task 8: " << taskTimes[7] / testCases.size() << " ns\n";

    cout << "Full task details written to parse_output.txt\n";

    // Write per-formula timings to file
    outFile << "\n\n=== Performance Summary ===\n";
    for (size_t i = 0; i < testCases.size(); ++i) {
        outFile << "Formula " << (i + 1) << " timings:\n";
        outFile << "T1: " << taskTimesPerFormula[i][0] << " ns\n";
        outFile << "T2: " << taskTimesPerFormula[i][1] << " ns\n";
        outFile << "T3: " << taskTimesPerFormula[i][2] << " ns\n";
        outFile << "T4: " << taskTimesPerFormula[i][3] << " ns\n";
        if(doEvaluateSpecific)
            outFile << "T5: " << taskTimesPerFormula[i][4] << " ns\n";
        outFile << "T6: " << taskTimesPerFormula[i][5] << " ns\n";
        outFile << "T7: " << taskTimesPerFormula[i][6] << " ns\n";
        if (doTruthTable)
            outFile << "T8: " << taskTimesPerFormula[i][7] << " ns\n";
        outFile << "---------------------------------------\n";
    }

    outFile.close();
    return 0;
}