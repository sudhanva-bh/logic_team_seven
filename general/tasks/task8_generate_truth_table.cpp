#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

#include "../common/parsetree_node.h"

namespace {
void getAtomsRecursive(Node* currentNode, set<char>& atoms) {
    if (currentNode == nullptr) {
        return;
    }
    char token = currentNode->data;
    if (token == '~') {
        getAtomsRecursive(currentNode->right, atoms);
    } else if (isOperator(token)) {
        getAtomsRecursive(currentNode->left, atoms);
        getAtomsRecursive(currentNode->right, atoms);
    } else {
        atoms.insert(token);
    }
}

void collectClausesAsVector(const Node* currentNode,
                            vector<const Node*>& clauses) {
    if (!currentNode) return;
    if (currentNode->data == '*') {
        collectClausesAsVector(currentNode->left, clauses);
        collectClausesAsVector(currentNode->right, clauses);
    } else {
        clauses.push_back(currentNode);
    }
}

void collectLiteralsAsSet(const Node* currentNode, set<char>& positive,
                          set<char>& negative) {
    if (!currentNode) return;
    if (currentNode->data == '+') {
        collectLiteralsAsSet(currentNode->left, positive, negative);
        collectLiteralsAsSet(currentNode->right, positive, negative);
    } else if (currentNode->data == '~') {
        if (currentNode->right) {
            negative.insert(currentNode->right->data);
        }
    } else {
        positive.insert(currentNode->data);
    }
}

void collectClausesWithLiterals(
    const Node* cnf_root, vector<pair<set<char>, set<char>>>& clause_literals) {
    vector<const Node*> clauses;
    collectClausesAsVector(cnf_root, clauses);

    for (const Node* clause : clauses) {
        set<char> positive, negative;
        collectLiteralsAsSet(clause, positive, negative);
        clause_literals.push_back({positive, negative});
    }
}

bool evaluateCNF(const vector<pair<set<char>, set<char>>>& clause_literals,
                 const map<char, bool>& truth_values) {
    for (const auto& clause : clause_literals) {
        const set<char>& positive = clause.first;
        const set<char>& negative = clause.second;

        bool clause_value = false;

        // OR over positive literals
        for (char p : positive) {
            auto it = truth_values.find(p);
            if (it != truth_values.end() && it->second) {
                clause_value = true;
                break;
            }
        }

        // OR over negated literals
        if (!clause_value) {
            for (char n : negative) {
                auto it = truth_values.find(n);
                if (it != truth_values.end() && !it->second) {
                    clause_value = true;
                    break;
                }
            }
        }

        // AND over clauses
        if (!clause_value) {
            return false;  // One clause is false => CNF is false
        }
    }
    return true;
}

void assignTruthValues(int bitNumber, map<char, bool>& truth_values,
                       int numOfAtoms) {
    // Extract keys into a vector
    vector<char> atoms;
    for (auto& kv : truth_values) {
        atoms.push_back(kv.first);
    }

    // Assign truth values in reverse order
    for (int i = 0; i < numOfAtoms; ++i) {
        bool bit = (bitNumber >> i) & 1;
        truth_values[atoms[numOfAtoms - 1 - i]] = bit;
    }
}

}  // namespace

void generateTruthTable(Node* cnfRootNode, set<char>& atoms) {
    getAtomsRecursive(cnfRootNode, atoms);
    int numOfAtoms = atoms.size();
    map<char, bool> truth_values;

    // Initialize all atoms to false
    for (char atom : atoms) {
        truth_values[atom] = false;
    }

    vector<pair<set<char>, set<char>>> clause_literals;
    collectClausesWithLiterals(cnfRootNode, clause_literals);

    int max = (1 << numOfAtoms) - 1;

    // Heading
    cout << string(numOfAtoms * 5 + 3, '-') << endl;
    for (char atom : atoms) {
        cout << left << setw(5) << atom;
    }
    cout << "OUT" << endl;
    cout << string(numOfAtoms * 5 + 3, '-') << endl;

    for (int i = max; i >= 0; i--) {
        assignTruthValues(i, truth_values, numOfAtoms);

        char result = evaluateCNF(clause_literals, truth_values) ? 'T' : 'F';

        for (auto& kv : truth_values) {
            cout << left << setw(5) << (kv.second ? 'T' : 'F');
        }

        cout << result << endl;
    }
}