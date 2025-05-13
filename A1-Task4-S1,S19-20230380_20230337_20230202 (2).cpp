#include "VoleMachine.h"
#include <iomanip> // For setw
#include <limits>  // For overflow limits
using namespace std;

VoleMachine::VoleMachine() : programCounter(0), instructionRegister(0) {
    registers.fill(0); // Initialize all registers to 0
    memory.fill(0);    // Initialize all memory to 0
}

bool VoleMachine::loadProgram(const string& filename, int startAddress) {
    ifstream file(filename);
    if (!file) {
        cout << "Error: Unable to open file " << filename << endl;
        return false;
    }
    instructions.clear();
    string line;
    int address = startAddress;
    while (getline(file, line) && address < memory.size()) {
        if (line.empty() || line[0] == ';') continue; // Ignore comments
        memory[address++] = stoi(line, nullptr, 16); // Store as integer
        cout << "Loaded instruction " << line << " at address " << address - 1 << endl;
    }
    cout << "Program loaded into memory starting at address " << startAddress << endl;
    return true;
}

void VoleMachine::execute() {
    while (programCounter < 256) { // 256 addresses in memory
        instructionRegister = memory[programCounter++];
        int opcode = (instructionRegister & 0xF000) >> 12;
        int R = (instructionRegister & 0x0F00) >> 8;
        int S = (instructionRegister & 0x00F0) >> 4;
        int T = instructionRegister & 0x000F;
        int XY = instructionRegister & 0x00FF;

        cout << "\n------------------------------------\n";
        cout << "Executing instruction: " << hex << instructionRegister
             << " | Opcode: " << opcode << " | R: " << R << " | S: " << S
             << " | T: " << T << " | XY: " << XY << dec << endl;

        switch (opcode) {
            case 1: // LOAD from memory
                registers[R] = memory[XY];
                cout << "Loaded value " << registers[R] << " from memory[" << XY << "]\n";
                break;
            case 2: // LOAD immediate
                registers[R] = XY;
                cout << "Loaded immediate value " << XY << " into R" << R << "\n";
                break;
            case 3: // STORE
                if (XY == 0) {
                    cout << "Screen Output: " << hex << registers[R] << dec << endl;
                } else {
                    memory[XY] = registers[R];
                    cout << "Stored R" << R << " value " << registers[R] 
                         << " into memory[" << XY << "]\n";
                }
                break;
            case 4: // MOVE
                registers[R] = registers[S];
                cout << "Moved value from R" << S << " to R" << R << "\n";
                break;
            case 5: // ADD and SUB with overflow handling
                if (S == 0) { // ADD operation with overflow check
                    int32_t addResult = static_cast<int32_t>(registers[R]) + registers[T];
                    if (addResult > numeric_limits<int16_t>::max()) {
                        addResult -= 65536; // Wrap around to simulate overflow
                    } else if (addResult < numeric_limits<int16_t>::min()) {
                        addResult += 65536;
                    }
                    registers[R] = static_cast<int16_t>(addResult);
                    cout << "Added R" << T << " to R" << R << " | New R" << R << ": " << registers[R] << "\n";
                } else if (S == 1) { // SUB operation with overflow check
                    int32_t subResult = static_cast<int32_t>(registers[R]) - registers[T];
                    if (subResult > numeric_limits<int16_t>::max()) {
                        subResult -= 65536; // Wrap around to simulate overflow
                    } else if (subResult < numeric_limits<int16_t>::min()) {
                        subResult += 65536;
                    }
                    registers[R] = static_cast<int16_t>(subResult);
                    cout << "Subtracted R" << T << " from R" << R << " | New R" << R << ": " << registers[R] << "\n";
                }
                break;
            case 6: // Logical operations
                switch (S) {
                    case 0: // AND
                        registers[R] &= registers[T];
                        break;
                    case 1: // OR
                        registers[R] |= registers[T];
                        break;
                    case 2: // XOR
                        registers[R] ^= registers[T];
                        break;
                    case 3: // NOT
                        registers[R] = ~registers[R];
                        break;
                }
                break;
            case 7: // Shift operations
                if (S == 0) {
                    registers[R] <<= 1; // Shift left
                } else if (S == 1) {
                    registers[R] >>= 1; // Shift right
                }
                break;
            case 8: // JUMP
                if (S == 0 && registers[R] == 0) { // Conditional jump
                    programCounter = XY;
                } else if (S == 1) { // Unconditional jump
                    programCounter = XY;
                }
                break;
            case 9: // Input/Output
                if (S == 0) { // INPUT
                    cout << "Enter value for R" << R << ": ";
                    cin >> registers[R];
                } else if (S == 1) { // OUTPUT
                    cout << "Output from R" << R << ": " << registers[R] << "\n";
                }
                break;
            case 11: // JUMP if R == S
                if (registers[R] == registers[S]) {
                    programCounter = XY;
                }
                break;
            case 12: // HALT
                return;
            default:
                cout << "Invalid opcode: " << opcode << endl;
                return;
        }
        displayStatus();
    }
}

void VoleMachine::displayStatus() const {
    cout << "\nCurrent Status:\n";
    cout << "Registers:\n";
    for (int i = 0; i < registers.size(); ++i) {
        cout << "R" << i << ": " << registers[i] << " ";
        if ((i + 1) % 4 == 0) cout << "\n";
    }
    cout << "Program Counter (PC): " << programCounter << "\n";
    cout << "Instruction Register (IR): " << hex << instructionRegister << dec << "\n";
    cout << "Memory:\n";
    for (int i = 0; i < 256; ++i) {
        cout << setw(4) << memory[i] << " ";
        if ((i + 1) % 16 == 0) cout << "\n";
    }
}

void VoleMachine::initializeMemoryWithData() {
    memory[0x10] = 0x5; // Example data
    memory[0x11] = 0x10;
}

void VoleMachine::setProgramCounter(int address) {
    programCounter = address;
}

void VoleMachine::displayMenu() const {
    cout << "1. Load Program\n2. Set Program Counter\n3. Execute\n4. Display Status\n5. Exit\n";
}
