#ifndef VOLE_MACHINE_H
#define VOLE_MACHINE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <array>
#include <cstdint>        // For int16_t and int32_t types
#include <limits>         // For numeric limits

using namespace std;
class VoleMachine {

public:
    array<int16_t, 16> registers;      // 16-bit registers R0 to R15
    array<int16_t, 256> memory;        // 16x16 Memory array
    int programCounter;                     // Points to the current instruction
    int instructionRegister;                // Holds the current instruction
    vector<string> instructions;  // Instructions as strings

    VoleMachine(); // Constructor

    // Load instructions from a file at a specific memory address
    bool loadProgram(const std::string& filename, int startAddress);

    // Execute the loaded instructions
    void execute();

    // Display status of registers, PC, IR, and memory as a 16x16 matrix
    void displayStatus() const;

    // Utility functions
    void initializeMemoryWithData();         // Preloads memory with test data
    void setProgramCounter(int address);     // Sets the program counter to a specific address
    void displayMenu() const;                // Displays the user menu for interacting with the Vole machine

private:
    // Helper function to handle overflow checks in arithmetic operations
    int16_t addWithOverflow(int16_t a, int16_t b) const;
    int16_t subWithOverflow(int16_t a, int16_t b) const;
};

#endif // VOLE_MACHINE_H
