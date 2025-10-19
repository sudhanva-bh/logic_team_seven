/**
 * @file generate_prepositional_logic_statements.cpp
 * @brief Main runner for the Logic Team Seven project.
 *
 * This file demonstrates the full pipeline of the logic toolkit.
 * It performs the following steps:
 * 1. Generates a random infix formula.
 * 2. (Task 1) Converts the infix formula to prefix.
 * 3. (Task 2) Builds a parse tree from the prefix string.
 * 4. (Task 3) Converts the tree back to infix for verification.
 * 5. (Task 4) Computes the height of the parse tree.
 * 6. (Task 5) Evaluates the tree with a sample set of truth values.
 * 7. (Task 6) Converts the original tree to Conjunctive Normal Form (CNF).
 * 8. (Task 7) Checks the validity (tautology) of the resulting CNF.
 * 9. (Task 8) Generates a full truth table for the CNF.
 *
 * It ensures all created trees are properly deleted to prevent memory leaks.
 */

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cstdlib> // For srand
#include <ctime>   // For time

// --- Core Project Headers ---
// Defines the Node struct and all helper functions (e.g., isOperator)
#include "common/parsetree_node.h"

// Declares all 8 task functions
#include "common/tasks.h"

// Declares the random formula generator
// (Ensure random_formula_generator.h is in your common folder or adjust path)
#include "common/random_formula_generator.h"

// Function to print a separator line
void printSeparator()
{
    std::cout << "\n"
              << std::string(60, '-') << "\n\n";
}

int main()
{
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));

    // --- Generate a Random Formula ---
    // Generate 1 formula, using 3 variables ('a', 'b', 'c'), min depth 2, max 3
    std::vector<string> formulas = generateRandomFormulas(1, 3, 2, 3);
    string infix = formulas[0];

    std::cout << "Starting Full Pipeline Test..." << std::endl;
    printSeparator();
    std::cout << "Original Infix:   " << infix << std::endl;

    // --- Task 1: Infix to Prefix ---
    string prefix = infixToPrefix(infix);
    std::cout << "Task 1 (Prefix):  " << prefix << std::endl;

    // --- Task 2: Prefix to Parse Tree ---
    Node *root = prefixToParseTree(prefix);
    std::cout << "Task 2 (Prefix to Tree): Success" << std::endl;

    // --- Task 3: Parse Tree to Infix ---
    string infix_check = parseTreeToInfix(root);
    std::cout << "Task 3 (Tree to Infix): " << infix_check << std::endl;

    // --- Task 4: Compute Height ---
    int height = computeHeightOfParseTree(root);
    std::cout << "Task 4 (Height):  " << height << std::endl;

    // --- Task 5: Evaluate Truth Value ---
    // Create a sample truth assignment for atoms 'a', 'b', 'c'
    map<char, bool> truthValues = {{'a', true}, {'b', false}, {'c', true}};
    bool result = evaluateTruthValue(root, truthValues);
    std::cout << "Task 5 (Evaluate 'a=T, b=F, c=T'): "
              << (result ? "True" : "False") << std::endl;

    printSeparator();
    std::cout << "Starting CNF Conversion..." << std::endl;

    // --- Task 6: Convert to CNF ---
    Node *cnfRoot = computeCnfFromParseTree(root);
    string cnf_infix = parseTreeToInfix(cnfRoot);
    std::cout << "Task 6 (CNF Tree): " << cnf_infix << std::endl;

    // --- Task 7: Check Validity ---
    int validClauses = 0;
    int invalidClauses = 0;
    bool is_valid = isValid(cnfRoot, validClauses, invalidClauses);
    std::cout << "Task 7 (Is Valid): " << (is_valid ? "Yes (Tautology)" : "No") << std::endl;
    std::cout << "         Valid Clauses: " << validClauses
              << " | Invalid Clauses: " << invalidClauses << std::endl;

    // --- Task 8: Generate Truth Table ---
    set<char> atoms;
    std::cout << "\nTask 8 (Truth Table for CNF):" << std::endl;
    generateTruthTable(cnfRoot, atoms);

    printSeparator();

    // --- Cleanup ---
    // IMPORTANT: Free all memory allocated for both trees
    std::cout << "Cleaning up memory..." << std::endl;
    destroyParseTree(root);
    destroyParseTree(cnfRoot);
    std::cout << "Pipeline complete." << std::endl;

    return 0;
}