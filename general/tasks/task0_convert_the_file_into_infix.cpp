#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Reads a CNF file and returns the contents as a string
string readCNFFile(const string &filepath) {
    ifstream file(filepath);
    if (!file.is_open()) throw runtime_error("Could not open file " + filepath);

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Converts CNF file contents to infix expression
string cnfToInfix(const string &cnfContents) {
    stringstream ss(cnfContents);
    string line;
    vector<string> clauses;

    while (getline(ss, line)) {
        if (line.empty() || line[0] == 'c' || line[0] == 'p') continue;

        stringstream lineSS(line);
        int literal;
        vector<string> clauseLiterals;

        while (lineSS >> literal) {
            if (literal == 0) break;
            clauseLiterals.push_back(literal < 0 ? "~x" + to_string(-literal)
                                                 : "x" + to_string(literal));
        }

        string clauseInfix;
        for (size_t i = 0; i < clauseLiterals.size(); ++i) {
            clauseInfix += clauseLiterals[i];
            if (i + 1 < clauseLiterals.size()) clauseInfix += "+";
        }
        clauses.push_back("(" + clauseInfix + ")");
    }

    string infix;
    for (size_t i = 0; i < clauses.size(); ++i) {
        infix += clauses[i];
        if (i + 1 < clauses.size()) infix += "*";
    }

    return infix;
}