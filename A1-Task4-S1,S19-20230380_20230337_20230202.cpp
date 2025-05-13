#include "VoleMachine.h"
#include <iostream> // For input and output
using namespace std;

int main() {
    VoleMachine vm;
    int choice;
    string filename;
    int address;

    while (true) {
        vm.displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter filename to load: ";
                cin >> filename;
                cout << "Enter start address in memory: ";
                cin >> hex >> address;
                if (!vm.loadProgram(filename, address)) {
                    cout << "Failed to load program.\n";
                }
                break;
            case 2:
                cout << "Enter start address for PC: ";
                cin >> hex >> address;
                vm.setProgramCounter(address);
                break;
            case 3:
                vm.execute();
                break;
            case 4:
                vm.displayStatus();
                break;
            case 5:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice.\n";
        }
    }
}
