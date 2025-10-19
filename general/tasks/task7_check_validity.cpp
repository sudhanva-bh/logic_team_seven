#include <set>
#include <vector>

#include "../common/parsetree_node.h"

using namespace std;

namespace {
void collectClauses(Node* currentCnfNode, vector<Node*>& clauses) {
    if (!currentCnfNode) return;
    if (currentCnfNode->data == '*') {
        collectClauses(currentCnfNode->left, clauses);
        collectClauses(currentCnfNode->right, clauses);
    } else {
        // Anything not a top-level AND is considered a clause
        clauses.push_back(currentCnfNode);
    }
}

void collectLiterals(Node* clauseNode, set<char>& positive,
                     set<char>& negative) {
    if (!clauseNode) return;
    if (clauseNode->data == '+') {
        collectLiterals(clauseNode->left, positive, negative);
        collectLiterals(clauseNode->right, positive, negative);
    } else if (clauseNode->data == '~') {
        if (clauseNode->right) {
            negative.insert(clauseNode->right->data);
        }
    } else {
        positive.insert(clauseNode->data);
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

bool isValid(Node* cnf_root, int& validCount, int& invalidCount) {
    validCount = 0;
    invalidCount = 0;

    if (!cnf_root) return true;

    vector<Node*> clauses;
    collectClauses(cnf_root, clauses);

    if (clauses.empty()) return true;

    for (Node* clause : clauses) {
        if (isTautologicalClause(clause))
            ++validCount;
        else
            ++invalidCount;
    }

    return invalidCount == 0;  // true if all are valid
}


