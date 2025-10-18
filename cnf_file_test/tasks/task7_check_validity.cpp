#include <set>
#include <vector>

#include "../common/parsetree_node.h"

using namespace std;

namespace {
void collectClauses(Node* currentNode, vector<Node*>& clauses) {
    if (!currentNode) return;
    if (currentNode->data == '*') {
        collectClauses(currentNode->left, clauses);
        collectClauses(currentNode->right, clauses);
    } else {
        // Anything not a top-level AND is considered a clause
        clauses.push_back(currentNode);
    }
}

void collectLiterals(Node* currentNode, set<char>& positive,
                     set<char>& negative) {
    if (!currentNode) return;
    if (currentNode->data == '+') {
        collectLiterals(currentNode->left, positive, negative);
        collectLiterals(currentNode->right, positive, negative);
    } else if (currentNode->data == '~') {
        if (currentNode->right) {
            negative.insert(currentNode->right->data);
        }
    } else {
        positive.insert(currentNode->data);
    }
}

bool isTautologicalClause(Node* clauseNode) {
    set<char> positive_literals;
    set<char> negative_literals;
    collectLiterals(clauseNode, positive_literals, negative_literals);

    for (char p_literal : positive_literals) {
        if (negative_literals.count(p_literal)) {
            return true;  // Found a complementary pair like p and ~p
        }
    }
    return false;
}
}  // namespace
bool isValid(Node* cnf_root) {
    if (!cnf_root) return true;  // An empty formula is valid.

    vector<Node*> clauses;
    collectClauses(cnf_root, clauses);

    if (clauses.empty()) return true;

    for (Node* clause : clauses) {
        if (!isTautologicalClause(clause)) {
            return false;  // If any clause is not a tautology, the formula is
                           // not valid.
        }
    }

    return true;  // All clauses were tautologies.
}