/**
 * @file task6_convert_to_cnf.cpp
 * @brief Converts a logical expression parse tree into Conjunctive Normal Form (CNF).
 *
 * @details This file implements the multi-step algorithm for CNF conversion:
 * 1. Eliminate implications (Implication-Free).
 * 2. Move negations inward to atoms (Negation Normal Form - NNF).
 * 3. Distribute OR operators over AND operators.
 *
 * This process generates a *new* tree that is logically equivalent
 * to the input tree and is in CNF.
 */

#include <algorithm>
#include <iostream> // for debug
#include <stack>
#include <string>

#include "../common/parsetree_node.h" // Assumed to contain Node and helpers
                                      // like negation(), disjunction(), etc.

using namespace std;

/**
 * @namespace
 * @brief Contains internal helper functions for the CNF conversion steps.
 */
namespace
{

    /**
     * @brief Step 1: Recursively removes all implication operators.
     *
     * @details Traverses the tree and replaces every sub-expression
     * `A > B` with its equivalent `~A + B`.
     * This function creates new nodes to form the new tree structure.
     *
     * @internal
     * @param currentNode The current node in the traversal.
     * @return A pointer to the root of the new, implication-free sub-tree.
     */
    Node *impl_free(Node *currentNode)
    {
        if (!currentNode)
            return nullptr;

        // Recursively process children first
        Node *left = impl_free(currentNode->left);
        Node *right = impl_free(currentNode->right);

        Node *result = nullptr;

        switch (currentNode->data)
        {
        case '>':
            // Replace (A > B) with (~A + B)
            result = disjunction(negation(left), right);
            break;
        case '~':
            result = negation(right);
            break;
        case '*':
            result = conjunction(left, right);
            break;
        case '+':
            result = disjunction(left, right);
            break;
        default:
            // Operand (leaf)
            result = new Node(currentNode->data);
            break;
        }
        return result;
    }

    /**
     * @brief Step 2: Recursively converts an implication-free tree to Negation Normal Form (NNF).
     *
     * @details Pushes all negation (`~`) operators inward so they only
     * apply to atomic propositions (operands).
     * Applies De Morgan's laws:
     * - `~(A * B)` becomes `~A + ~B`
     * - `~(A + B)` becomes `~A * ~B`
     * Also removes double negations:
     * - `~~A` becomes `A`
     *
     * @internal
     * @param currentNode The current node in the traversal (must be implication-free).
     * @return A pointer to the root of the new, NNF sub-tree.
     */
    Node *nnf(Node *currentNode)
    {
        if (!currentNode)
            return nullptr;

        Node *result = nullptr;

        switch (currentNode->data)
        {
        case '+':
            // NNF(A + B) => NNF(A) + NNF(B)
            result =
                disjunction(nnf(currentNode->left), nnf(currentNode->right));
            break;
        case '*':
            // NNF(A * B) => NNF(A) * NNF(B)
            result =
                conjunction(nnf(currentNode->left), nnf(currentNode->right));
            break;
        case '~':
        {
            Node *innerNode = currentNode->right;
            switch (innerNode->data)
            {
            case '~': // ~(~A) => NNF(A)
                result = nnf(innerNode->right);
                break;
            case '+': // ~(A + B) => NNF(~A) * NNF(~B)
                result = conjunction(nnf(negation(innerNode->left)),
                                     nnf(negation(innerNode->right)));
                break;
            case '*': // ~(A * B) => NNF(~A) + NNF(~B)
                result = disjunction(nnf(negation(innerNode->left)),
                                     nnf(negation(innerNode->right)));
                break;
            default: // ~A (A is an atom)
                result = negation(innerNode);
                break;
            }
            break;
        }
        default:
            // Operand (leaf)
            result = new Node(currentNode->data);
            break;
        }
        return result;
    }

    /**
     * @brief Step 3 (Helper): Recursively distributes OR over AND.
     *
     * @details This is the core logic for the final CNF step.
     * It applies the distributive law:
     * - `(A * B) + C` becomes `(A + C) * (B + C)`
     * - `A + (B * C)` becomes `(A + B) * (A + C)`
     *
     * @internal
     * @param node1 The left-hand side of a disjunction (A).
     * @param node2 The right-hand side of a disjunction (B or C).
     * @param depth Current recursion depth, for stack overflow protection.
     * @return A pointer to the root of the new, distributed sub-tree.
     */
    Node *distr(Node *node1, Node *node2, int depth = 0)
    {
        if (!node1 || !node2)
            return nullptr;

        // Safety check to prevent stack overflow on deep recursion
        const int MAX_DEPTH = 2000; // tune as needed
        if (depth > MAX_DEPTH)
        {
            "disjunction copy\n"; // Debug string literal, no-op
            return disjunction(copyNode(node1), copyNode(node2));
        }

        // If left is a conjunction, distribute: (A*B)+C => (A+C)*(B+C)
        if (node1->data == '*')
        {
            Node *a = distr(node1->left, node2, depth + 1);
            Node *b = distr(node1->right, node2, depth + 1);
            return conjunction(a, b);
        }

        // If right is a conjunction, distribute: A+(B*C) => (A+B)*(A+C)
        if (node2->data == '*')
        {
            Node *a = distr(node1, node2->left, depth + 1);
            Node *b = distr(node1, node2->right, depth + 1);
            return conjunction(a, b);
        }

        // Base case: Neither side is a conjunction.
        return disjunction(copyNode(node1), copyNode(node2));
    }

    /**
     * @brief Step 3 (Main): Recursively applies the CNF distribution.
     *
     * @details Traverses the NNF tree. When it finds a disjunction (`+`),
     * it calls `distr` to handle the distribution of OR over AND.
     *
     * @internal
     * @param currentNode The current node in the NNF tree.
     * @param depth Current recursion depth.
     * @return A pointer to the root of the new, CNF sub-tree.
     */
    Node *cnf(Node *currentNode, int depth = 0)
    {
        if (!currentNode)
            return nullptr;

        const int MAX_DEPTH = 2000;
        string indent(depth * 2, ' '); // Debug variable, unused in logic

        if (depth > MAX_DEPTH)
        {
            return copyNode(currentNode);
        }

        Node *result = nullptr;

        switch (currentNode->data)
        {
        case '*':
        {
            // CNF(A * B) => CNF(A) * CNF(B)
            Node *leftCNF = cnf(currentNode->left, depth + 1);
            Node *rightCNF = cnf(currentNode->right, depth + 1);
            result = conjunction(leftCNF, rightCNF);
            break;
        }
        case '+':
        {
            // CNF(A + B) => distr(CNF(A), CNF(B))
            Node *leftCNF = cnf(currentNode->left, depth + 1);
            Node *rightCNF = cnf(currentNode->right, depth + 1);
            result = distr(leftCNF, rightCNF, depth + 1);
            break;
        }
        default:
            // Literal (atom or ~atom)
            result = copyNode(currentNode); // Always return a safe copy
            break;
        }

        return result;
    }

} // namespace

/**
 * @brief Public interface to convert a parse tree to CNF.
 *
 * @details Runs the complete three-step conversion:
 * 1.  `impl_free`: Removes implications.
 * 2.  `nnf`: Converts to Negation Normal Form.
 * 3.  `cnf`: Distributes ORs over ANDs to produce the final CNF.
 *
 * @param rootNode The root of the original logical expression parse tree.
 * @return A pointer to the root of a *new* tree in CNF.
 */
Node *computeCnfFromParseTree(Node *rootNode)
{
    if (!rootNode)
        return nullptr;

    // Step 1: Remove implications
    Node *implFreeTree = impl_free(rootNode);

    // Step 2: Convert to NNF
    Node *nnfTree = nnf(implFreeTree);

    // Step 3: Apply distributive laws to get CNF
    Node *cnfTree = cnf(nnfTree);

    return cnfTree;
}