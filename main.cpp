#include <iostream>
#include <string>
#include <conio.h>  // For _getch() on Windows

#include "cnf/runCNF.cpp"
#include "general/runGeneral.cpp"

using namespace std;

bool endsWith(const string& str, const string& suffix) {
    if (str.size() < suffix.size()) return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// Simple arrow-key menu (Windows)
int showMenu() {
    string options[] = {
        "CNF file processing",
        "General formula input"
    };
    int selected = 0;
    const int numOptions = 2;

    while (true) {
        system("cls");  // Use "clear" if on Linux/macOS
        cout << "Use UP and DOWN to navigate, Enter to select\n\n";

        for (int i = 0; i < numOptions; ++i) {
            if (i == selected)
                cout << "> " << options[i] << endl;
            else
                cout << "  " << options[i] << endl;
        }

        int key = _getch();

        // Arrow keys send 224 then a code
        if (key == 224) {
            key = _getch();
            if (key == 72) selected = (selected - 1 + numOptions) % numOptions; // Up
            else if (key == 80) selected = (selected + 1) % numOptions;         // Down
        } 
        else if (key == 13) { // Enter key
            return selected + 1; // Return 1 or 2
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        string filePath = argv[1];

        if (endsWith(filePath, ".cnf")) {
            return runCNF(filePath);
        } 
        else if (endsWith(filePath, ".txt")) {
            return runGeneral(filePath);
        } 
        else {
            cerr << "Unsupported file type! Please provide a .cnf or .txt file."
                 << endl;
            return 1;
        }
    } 
    else {
        int choice = showMenu();

        if (choice == 1) {
            return runCNF("");
        } 
        else if (choice == 2) {
            return runGeneral("");
        } 
        else {
            cerr << "Invalid choice!" << endl;
            return 1;
        }
    }
}
