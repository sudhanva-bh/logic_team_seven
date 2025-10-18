#include <set>
#include <vector>

#include "../common/parsetree_node.h"

using namespace std;

namespace {
vector<vector<int>> collectClauses(const string& cnfString) {
    vector<vector<int>> clauses;
    istringstream iss(cnfString);
    string line;

    while (getline(iss, line)) {
        // Skip comments and metadata lines (starting with 'c' or 'p')
        if (line.empty() || line[0] == 'c' || line[0] == 'p') continue;

        istringstream lineStream(line);
        int literal;
        vector<int> clause;

        while (lineStream >> literal) {
            if (literal == 0) {
                // Clause end marker
                if (!clause.empty()) {
                    clauses.push_back(clause);
                    clause.clear();
                }
            } else {
                clause.push_back(literal);
            }
        }
    }

    return clauses;
}
}  // namespace

bool isValid(const string& cnfString, int& validCount, int& invalidCount) {
    validCount = 0;
    invalidCount = 0;
    vector<vector<int>> clauses = collectClauses(cnfString);

    for (const auto& clause : clauses) {
        set<int> literals(clause.begin(), clause.end());
        bool clauseValid = false;

        for (int lit : literals) {
            if (literals.count(-lit)) {
                clauseValid = true;
                break;
            }
        }

        if (clauseValid)
            ++validCount;
        else
            ++invalidCount;
    }

    // Return true if all are valid
    return invalidCount == 0;
}