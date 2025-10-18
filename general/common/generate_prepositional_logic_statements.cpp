#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

namespace {
const std::string allAtoms = "abcdefghijklmnopqrstuvwxyz";

char randomAtom(const std::string& atoms) {
    return atoms[rand() % atoms.size()];
}

char randomBinaryOp() {
    const std::string ops = "+*>";  // OR, AND, IMPLIES
    return ops[rand() % ops.size()];
}

bool applyNegation(double negProb) {
    return ((rand() % 100) / 100.0) < negProb;
}

std::string generateFormulaRec(int depth, int minDepth, int maxDepth,
                               const std::string& atoms, double negProb) {
    // Base case: if maxDepth reached or probabilistically stop
    if (depth >= maxDepth || (depth >= minDepth && rand() % 2 == 0)) {
        std::string lit = "";
        if (applyNegation(negProb)) lit += "~";
        lit += randomAtom(atoms);
        return lit;
    }

    std::string left =
        generateFormulaRec(depth + 1, minDepth, maxDepth, atoms, negProb);
    std::string right =
        generateFormulaRec(depth + 1, minDepth, maxDepth, atoms, negProb);
    char op = randomBinaryOp();
    return "(" + left + op + right + ")";
}
}  // namespace

// Public interface
std::vector<std::string> generateRandomFormulas(int numFormulas,
                                                int numVariables = 5,
                                                int minDepth = 2,
                                                int maxDepth = 5,
                                                double negationProb = 0.5) {
    // Select first numVariables letters
    std::string atoms = allAtoms.substr(0, numVariables);

    std::vector<std::string> formulas;
    for (int i = 0; i < numFormulas; ++i) {
        formulas.push_back(
            generateFormulaRec(0, minDepth, maxDepth, atoms, negationProb));
    }
    return formulas;
}