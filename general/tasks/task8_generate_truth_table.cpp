/**
 * @file task8_generate_truth_table.cpp
 * @brief Generates and prints a truth table for a CNF formula.
 *
 * @details This file provides functionality to:
 * 1. Find all unique atomic propositions (variables) in a formula.
 * 2. Iterate through all $2^N$ possible truth assignments for N atoms.
 * 3. Evaluate the CNF formula for each assignment.
 * 4. Print a formatted truth table to standard output.
 *
 * It uses an efficient pre-processing step to evaluate the CNF
 * repeatedly.
 */

#include <iomanip>  // For setw, left
#include <iostream> // For cout, endl
#include <map>
#include <set>
#include <string>
#include <vector>

#include "../common/parsetree_node.h" // Assumed to contain Node, isOperator

using namespace std;

/**
 * @namespace
 * @brief Contains internal helper functions for truth table generation.
 */
namespace
{
    /**
     * @brief Recursively collects all unique atoms (variables) from a tree.
     *
     * @internal
     * @param currentNode The current node in the traversal.
     * @param atoms An output set to store the unique atoms found.
     */
    void getAtomsRecursive(Node *currentNode, set<char> &atoms)
    {
        if (currentNode == nullptr)
        {
            return;
        }
        char token = currentNode->data;
        if (token == '~')
        {
            getAtomsRecursive(currentNode->right, atoms);
        }
        else if (isOperator(token))
        {
            getAtomsRecursive(currentNode->left, atoms);
            getAtomsRecursive(currentNode->right, atoms);
        }
        else
        {
            // It's an operand (atom), insert it into the set.
            atoms.insert(token);
        }
    }

    /**
     * @brief Collects all clauses from a CNF tree into a vector.
     *
     * @details (Same as in task7) Traverses the top-level ANDs
     * to get a list of all clause sub-trees.
     *
     * @internal
     * @param currentNode The current node in the CNF tree.
     * @param clauses An output vector to store pointers to clause nodes.
     */
    void collectClausesAsVector(const Node *currentNode,
                                vector<const Node *> &clauses)
    {
        if (!currentNode)
            return;
        if (currentNode->data == '*')
        {
            collectClausesAsVector(currentNode->left, clauses);
            collectClausesAsVector(currentNode->right, clauses);
        }
        else
        {
            clauses.push_back(currentNode);
        }
    }

    /**
     * @brief Collects positive and negative literals from a clause tree.
     *
     * @details (Same as in task7) Traverses a clause (ORs) and
     * populates sets of positive and negative literals.
     *
     * @internal
     * @param currentNode The root of the clause sub-tree.
     * @param positive An output set for positive literals.
     * @param negative An output set for negative literals.
     */
    void collectLiteralsAsSet(const Node *currentNode, set<char> &positive,
                              set<char> &negative)
    {
        if (!currentNode)
            return;
        if (currentNode->data == '+')
        {
            collectLiteralsAsSet(currentNode->left, positive, negative);
            collectLiteralsAsSet(currentNode->right, positive, negative);
        }
        else if (currentNode->data == '~')
        {
            if (currentNode->right)
            {
                negative.insert(currentNode->right->data);
            }
        }
        else
        {
            positive.insert(currentNode->data);
        }
    }

    /**
     * @brief Pre-processes a CNF tree into an efficient evaluation structure.
     *
     * @details Converts the entire CNF tree into a `vector` of clauses,
     * where each clause is represented by a `pair` of `set`s:
     * one for its positive literals and one for its negative literals.
     * This is much faster to evaluate repeatedly than traversing the tree.
     *
     * @internal
     * @param cnf_root The root of the CNF tree.
     * @param clause_literals An output vector to store the pre-processed clauses.
     */
    void collectClausesWithLiterals(
        const Node *cnf_root, vector<pair<set<char>, set<char>>> &clause_literals)
    {
        vector<const Node *> clauses;
        collectClausesAsVector(cnf_root, clauses);

        for (const Node *clause : clauses)
        {
            set<char> positive, negative;
            collectLiteralsAsSet(clause, positive, negative);
            clause_literals.push_back({positive, negative});
        }
    }

    /**
     * @brief Evaluates a pre-processed CNF formula given a set of truth values.
     *
     * @details Checks if the CNF formula is true for the given assignment.
     * A CNF is true if *all* of its clauses are true.
     * A clause is true if *at least one* of its literals is true.
     *
     * @internal
     * @param clause_literals The pre-processed CNF data structure.
     * @param truth_values A map of atoms to their current T/F values.
     * @return true if the formula is satisfied, false otherwise.
     */
    bool evaluateCNF(const vector<pair<set<char>, set<char>>> &clause_literals,
                     const map<char, bool> &truth_values)
    {
        // Iterate over all clauses (Conjunction)
        for (const auto &clause : clause_literals)
        {
            const set<char> &positive = clause.first;
            const set<char> &negative = clause.second;

            bool clause_value = false; // Is this clause true?

            // Check all positive literals (Disjunction)
            for (char p : positive)
            {
                auto it = truth_values.find(p);
                if (it != truth_values.end() && it->second)
                { // if p is 'T'
                    clause_value = true;
                    break;
                }
            }

            // Check all negative literals (Disjunction)
            if (!clause_value)
            {
                for (char n : negative)
                {
                    auto it = truth_values.find(n);
                    if (it != truth_values.end() && !it->second)
                    { // if n is 'F' (so ~n is 'T')
                        clause_value = true;
                        break;
                    }
                }
            }

            // If even one clause is false, the whole CNF is false
            if (!clause_value)
            {
                return false;
            }
        }
        // All clauses were true, so the CNF is true
        return true;
    }

    /**
     * @brief Assigns truth values to the atom map based on a row index.
     *
     * @details Uses bit manipulation to set the T/F value for each
     * atom, corresponding to one row of the truth table.
     *
     * @internal
     * @param bitNumber The row index (e.g., 0 to $2^N - 1$).
     * @param truth_values The map of atom values to update.
     * @param numOfAtoms The total number of atoms.
     */
    void assignTruthValues(int bitNumber, map<char, bool> &truth_values,
                           int numOfAtoms)
    {
        // Extract keys into a vector to ensure consistent order
        vector<char> atoms;
        for (auto &kv : truth_values)
        {
            atoms.push_back(kv.first);
        }

        // Assign truth values based on the bits of bitNumber
        for (int i = 0; i < numOfAtoms; ++i)
        {
            bool bit = (bitNumber >> i) & 1;
            // Assign bits in reverse to match standard truth table order
            truth_values[atoms[numOfAtoms - 1 - i]] = bit;
        }
    }

} // namespace

/**
 * @brief Public interface to generate and print a truth table for a CNF formula.
 *
 * @details Orchestrates the entire process:
 * 1. Finds all atoms.
 * 2. Pre-processes the CNF tree into an efficient list of clauses.
 * 3. Prints the table header.
 * 4. Iterates from $2^N - 1$ down to 0, assigning truth values
 * and evaluating the formula for each row.
 * 5. Prints each row.
 *
 * @param cnfRootNode The root of the CNF parse tree.
 * @param atoms An output parameter (passed by reference) that will
 * be populated with the set of all atoms found.
 */
void generateTruthTable(Node *cnfRootNode, set<char> &atoms)
{
    getAtomsRecursive(cnfRootNode, atoms);
    int numOfAtoms = atoms.size();
    map<char, bool> truth_values;

    // Initialize map with all atoms (defaulting to false)
    for (char atom : atoms)
    {
        truth_values[atom] = false;
    }

    // Pre-process the CNF tree for efficient evaluation
    vector<pair<set<char>, set<char>>> clause_literals;
    collectClausesWithLiterals(cnfRootNode, clause_literals);

    int max = (1 << numOfAtoms) - 1; // e.g., for 3 atoms, max is 7 (111)

    // Print table header
    cout << string(numOfAtoms * 5 + 3, '-') << endl;
    for (char atom : atoms)
    {
        cout << left << setw(5) << atom;
    }
    cout << "OUT" << endl;
    cout << string(numOfAtoms * 5 + 3, '-') << endl;

    // Iterate through all $2^N$ rows
    for (int i = max; i >= 0; i--)
    {
        assignTruthValues(i, truth_values, numOfAtoms);

        // Evaluate and get the result for this row
        char result = evaluateCNF(clause_literals, truth_values) ? 'T' : 'F';

        // Print the T/F values for each atom
        for (auto &kv : truth_values)
        {
            cout << left << setw(5) << (kv.second ? 'T' : 'F');
        }

        // Print the final result for the row
        cout << result << endl;
    }
}


