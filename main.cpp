#include <iostream>

// Forward declarations
int runCNF();
int runGeneral();

int main() {
    std::cout << "Choose mode:\n";
    std::cout << "1. CNF file processing\n";
    std::cout << "2. General formula input\n";
    std::cout << "Enter choice (1/2): ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(); // flush newline

    if (choice == 1) {
        return runCNF();
    } else if (choice == 2) {
        return runGeneral();
    } else {
        std::cerr << "Invalid choice!" << std::endl;
        return 1;
    }
}
