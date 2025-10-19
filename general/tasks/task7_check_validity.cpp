/**
 * @file task7_check_validity.cpp
 * @brief Checks if a CNF formula is valid (a tautology).
 *
 * @details A logical formula is valid (a tautology) if it is true
 * under all possible truth assignments.
 * For a formula in CNF, this is true if and only if *every*
 * clause within it is a tautology (i.e., every clause
 * contains a complementary pair of literals, like `A` and `~A`).
 */

#include <set>
#include <vector>

#include "../common/parsetree_node.h" // Assumed to contain Node

using namespace std;

/**
 * @namespace
 * @brief Contains internal helper functions for validity checking.
 */
namespace {
/**
 * @brief Collects all clauses from a CNF tree.
 *
 * @details A CNF tree is a top-level conjunction (`*`) of clauses.
 * This function traverses the tree and collects all the sub-trees
 * that are arguments to the top-level `*` operators.
 *
 * @internal
 * @param currentCnfNode The current node in the CNF tree.
 * @param clauses A reference to a vector to store the clause nodes.
 */
void collectClauses(Node* currentCnfNode, vector<Node*>& clauses) {
    if (!currentCnfNode) return;
    if (currentCnfNode->data == '*') {
        // This is an AND, so its children are either clauses or more ANDs
        collectClauses(currentCnfNode->left, clauses);
        collectClauses(currentCnfNode->right, clauses);
    } else {
        // This is not an AND, so it's a clause (e.g., A, ~A, A+B)
        clauses.push_back(currentCnfNode);
    }
}

/**
 * @brief Collects all positive and negative literals within a single clause.
 *
 * @details A clause is a disjunction (`+`) of literals.
 * This function traverses the clause tree and stores the literals
 * in two sets: one for positive literals (e.g., `A`) and one for
 * negative literals (e.g., `B` from `~B`).
 *
 * @internal
 * @param clauseNode The root node of the clause sub-tree.
 * @param positive Output set for positive literals (e.g., 'A').
 * @param negative Output set for negative literals (e.g., 'B' for ~B).
 */
void collectLiterals(Node* clauseNode, set<char>& positive,
                     set<char>& negative) {
    if (!clauseNode) return;
    if (clauseNode->data == '+') {
        // This is an OR, process both sides
        collectLiterals(clauseNode->left, positive, negative);
        collectLiterals(clauseNode->right, positive, negative);
    } else if (clauseNode->data == '~') {
        // This is a NOT, add its child to the negative set
        if (clauseNode->right) {
            negative.insert(clauseNode->right->data);
        }
    } else {
        // This is an atom, add to the positive set
        positive.insert(clauseNode->data);
    }
}

/**
 * @brief Checks if a single clause is a tautology.
 *
 * @details A clause is a tautology if and only if it contains
 * at least one complementary pair of literals (e.g., `A` and `~A`).
 *
 * @internal
 * @param clauseNode The root node of the clause to check.
 * @return true if the clause is a tautology, false otherwise.
 */
bool isTautologicalClause(Node* clauseNode) {
    set<char> positive_literals;
    set<char> negative_literals;
    collectLiterals(clauseNode, positive_literals, negative_literals);

    // Check for any intersection between the two sets
    for (char p_literal : positive_literals) {
        if (negative_literals.count(p_literal)) {
            return true;  // Found a pair like p and ~p
        }
    }
    return false;
}
}  // namespace

/**
 * @brief Public interface to check if a CNF formula is valid (a tautology).
 *
 * @details Traverses the CNF tree, breaks it into clauses, and checks
 * if *every* clause is a tautology. If even one clause is not
 * tautological, the entire formula is not valid.
 *
 * @param cnf_root The root node of the CNF expression tree.
 * @param validCount An output parameter (passed by reference) that will
 * be populated with the count of tautological clauses.
 * @param invalidCount An output parameter (passed by reference) that will
 * be populated with the count of non-tautological clauses.
 * @return true if the CNF formula is valid (all clauses are tautologies),
 * false otherwise.
 */
bool isValid(Node* cnf_root, int& validCount, int& invalidCount) {
    validCount = 0;
    invalidCount = 0;

    if (!cnf_root) return true; // An empty formula is valid

    vector<Node*> clauses;
    collectClauses(cnf_root, clauses);

    if (clauses.empty()) return true; // Also valid

    for (Node* clause : clauses) {
        if (isTautologicalClause(clause))
            ++validCount;
        else
            ++invalidCount;
    }

    // The formula is valid if and only if there are no invalid clauses.
    return invalidCount == 0;
}