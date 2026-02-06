#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Class to handle string utilities
class StringUtils {
public:
    // Remove comments from a line (everything after ';')
    static string removeComments(string line) {
        size_t pos = line.find(';');
        if (pos != string::npos) {
            line = line.substr(0, pos);
        }
        return line;
    }
    
    // Trim whitespace from both ends
    static string trim(string str) {
        size_t start = 0;
        size_t end = str.length();
        
        while (start < end && (str[start] == ' ' || str[start] == '\t' || str[start] == '\r')) {
            start++;
        }
        while (end > start && (str[end-1] == ' ' || str[end-1] == '\t' || str[end-1] == '\r')) {
            end--;
        }
        
        return str.substr(start, end - start);
    }
    
    // Check if string is a number
    static bool isNumber(string str) {
        if (str.empty()) return false;
        
        size_t start = 0;
        if (str[0] == '-' || str[0] == '+') start = 1;
        
        for (size_t i = start; i < str.length(); i++) {
            if (str[i] < '0' || str[i] > '9') {
                return false;
            }
        }
        return start < str.length();
    }
    
    // Convert string to integer
    static int stringToInt(string str) {
        int result = 0;
        int sign = 1;
        size_t i = 0;
        
        if (str[0] == '-') {
            sign = -1;
            i = 1;
        } else if (str[0] == '+') {
            i = 1;
        }
        
        for (; i < str.length(); i++) {
            result = result * 10 + (str[i] - '0');
        }
        
        return result * sign;
    }
};

// Class to represent a parsed instruction
class Instruction {
private:
    vector<string> parts;
    string originalLine;
    
public:
    // Constructor
    Instruction(string line) {
        originalLine = line;
        parseLine(line);
    }
    
    // Parse the instruction line into parts
    void parseLine(string line) {
        line = StringUtils::removeComments(line);
        line = StringUtils::trim(line);
        
        if (line.empty()) {
            return;
        }
        
        string current = "";
        for (size_t i = 0; i < line.length(); i++) {
            if (line[i] == ' ' || line[i] == ',' || line[i] == '\t') {
                if (!current.empty()) {
                    parts.push_back(current);
                    current = "";
                }
            } else {
                current += line[i];
            }
        }
        
        if (!current.empty()) {
            parts.push_back(current);
        }
    }
    
    // Get the instruction name (MOV, ADD, etc.)
    string getName() {
        if (parts.empty()) return "";
        return parts[0];
    }
    
    // Get operand at index
    string getOperand(int index) {
        if (index < 0 || index >= (int)parts.size() - 1) return "";
        return parts[index + 1];
    }
    
    // Get number of operands
    int getOperandCount() {
        if (parts.empty()) return 0;
        return parts.size() - 1;
    }
    
    // Check if instruction is empty
    bool isEmpty() {
        return parts.empty();
    }
    
    // Get the original line
    string getOriginalLine() {
        return StringUtils::trim(StringUtils::removeComments(originalLine));
    }
};

// Class to represent the CPU
class CPU {
private:
    int registers[6];      // 6 registers: R1-R6
    int programCounter;    // Current instruction line
    bool running;          // Is CPU running?
    int executionOption;   // 0=silent, 1=verbose
    
public:
    // Constructor
    CPU() {
        // Initialize all registers to 0
        for (int i = 0; i < 6; i++) {
            registers[i] = 0;
        }
        programCounter = 0;
        running = false;
        executionOption = 0;
    }
    
    // Get register index from name (R1->0, R2->1, etc.)
    int getRegisterIndex(string regName) {
        if (regName.length() == 2 && regName[0] == 'R') {
            int index = regName[1] - '1';
            if (index >= 0 && index < 6) {
                return index;
            }
        }
        return -1;
    }
    
    // Get register value
    int getRegisterValue(int index) {
        if (index >= 0 && index < 6) {
            return registers[index];
        }
        return 0;
    }
    
    // Set register value
    void setRegisterValue(int index, int value) {
        if (index >= 0 && index < 6) {
            registers[index] = value;
        }
    }
    
    // Print all register values
    void printRegisters() {
        cout << "R1=" << registers[0] 
             << ", R2=" << registers[1] 
             << ", R3=" << registers[2] 
             << ", R4=" << registers[3] 
             << ", R5=" << registers[4] 
             << ", R6=" << registers[5] << endl;
    }
    
    // Execute MOV instruction
    bool executeMOV(Instruction& inst) {
        if (inst.getOperandCount() != 2) {
            cout << "Error: MOV requires 2 operands" << endl;
            return false;
        }
        
        int destReg = getRegisterIndex(inst.getOperand(0));
        if (destReg == -1) {
            cout << "Error: Invalid destination register" << endl;
            return false;
        }
        
        // Check if source is register or constant
        int srcReg = getRegisterIndex(inst.getOperand(1));
        if (srcReg != -1) {
            registers[destReg] = registers[srcReg];
        } else if (StringUtils::isNumber(inst.getOperand(1))) {
            registers[destReg] = StringUtils::stringToInt(inst.getOperand(1));
        } else {
            cout << "Error: Invalid source operand" << endl;
            return false;
        }
        
        return true;
    }
    
    // Execute ADD instruction
    bool executeADD(Instruction& inst) {
        if (inst.getOperandCount() != 2) {
            cout << "Error: ADD requires 2 operands" << endl;
            return false;
        }
        
        int destReg = getRegisterIndex(inst.getOperand(0));
        if (destReg == -1) {
            cout << "Error: Invalid destination register" << endl;
            return false;
        }
        
        // Check if source is register or constant
        int srcReg = getRegisterIndex(inst.getOperand(1));
        if (srcReg != -1) {
            registers[destReg] += registers[srcReg];
        } else if (StringUtils::isNumber(inst.getOperand(1))) {
            registers[destReg] += StringUtils::stringToInt(inst.getOperand(1));
        } else {
            cout << "Error: Invalid source operand" << endl;
            return false;
        }
        
        return true;
    }
    
    // Execute SUB instruction
    bool executeSUB(Instruction& inst) {
        if (inst.getOperandCount() != 2) {
            cout << "Error: SUB requires 2 operands" << endl;
            return false;
        }
        
        int destReg = getRegisterIndex(inst.getOperand(0));
        if (destReg == -1) {
            cout << "Error: Invalid destination register" << endl;
            return false;
        }
        
        // Check if source is register or constant
        int srcReg = getRegisterIndex(inst.getOperand(1));
        if (srcReg != -1) {
            registers[destReg] -= registers[srcReg];
        } else if (StringUtils::isNumber(inst.getOperand(1))) {
            registers[destReg] -= StringUtils::stringToInt(inst.getOperand(1));
        } else {
            cout << "Error: Invalid source operand" << endl;
            return false;
        }
        
        return true;
    }
    
    // Execute JMP instruction
    int executeJMP(Instruction& inst) {
        // JMP lineAddress (unconditional)
        if (inst.getOperandCount() == 1) {
            if (StringUtils::isNumber(inst.getOperand(0))) {
                return StringUtils::stringToInt(inst.getOperand(0)) - 1;
            } else {
                cout << "Error: Invalid line address" << endl;
                return -1;
            }
        }
        // JMP reg, lineAddress (conditional)
        else if (inst.getOperandCount() == 2) {
            int reg = getRegisterIndex(inst.getOperand(0));
            if (reg == -1) {
                cout << "Error: Invalid register" << endl;
                return -1;
            }
            
            if (StringUtils::isNumber(inst.getOperand(1))) {
                if (registers[reg] == 0) {
                    return StringUtils::stringToInt(inst.getOperand(1)) - 1;
                } else {
                    return programCounter + 1;
                }
            } else {
                cout << "Error: Invalid line address" << endl;
                return -1;
            }
        }
        
        cout << "Error: JMP requires 1 or 2 operands" << endl;
        return -1;
    }
    
    // Execute PRN instruction
    bool executePRN(Instruction& inst) {
        if (inst.getOperandCount() != 1) {
            cout << "Error: PRN requires 1 operand" << endl;
            return false;
        }
        
        // Check if operand is register or constant
        int reg = getRegisterIndex(inst.getOperand(0));
        if (reg != -1) {
            cout << registers[reg] << endl;
        } else if (StringUtils::isNumber(inst.getOperand(0))) {
            cout << StringUtils::stringToInt(inst.getOperand(0)) << endl;
        } else {
            cout << "Error: Invalid operand" << endl;
            return false;
        }
        
        return true;
    }
    
    // Execute INP instruction
    bool executeINP(Instruction& inst) {
        if (inst.getOperandCount() != 1) {
            cout << "Error: INP requires 1 operand" << endl;
            return false;
        }
        
        int reg = getRegisterIndex(inst.getOperand(0));
        if (reg == -1) {
            cout << "Error: Invalid register" << endl;
            return false;
        }
        
        char ch;
        cin >> ch;
        registers[reg] = (int)ch;
        
        return true;
    }
    
    // Execute HLT instruction
    void executeHLT() {
        if (executionOption == 1) {
            printRegisters();
        }
        cout << "Program halted." << endl;
        printRegisters();
        running = false;
    }
    
    // Execute a single instruction
    bool executeInstruction(Instruction& inst) {
        string name = inst.getName();
        
        // Option 1: Print instruction before execution
        if (executionOption == 1) {
            cout << inst.getOriginalLine() << " - ";
        }
        
        bool success = true;
        int nextPC = programCounter + 1;
        
        if (name == "MOV") {
            success = executeMOV(inst);
        } 
        else if (name == "ADD") {
            success = executeADD(inst);
        } 
        else if (name == "SUB") {
            success = executeSUB(inst);
        } 
        else if (name == "JMP") {
            nextPC = executeJMP(inst);
            if (nextPC == -1) success = false;
        } 
        else if (name == "PRN") {
            success = executePRN(inst);
        } 
        else if (name == "INP") {
            success = executeINP(inst);
        } 
        else if (name == "HLT") {
            executeHLT();
            return true;
        } 
        else {
            cout << "Error: Unknown instruction '" << name << "'" << endl;
            return false;
        }
        
        if (!success) {
            return false;
        }
        
        // Option 1: Print registers after execution
        if (executionOption == 1) {
            printRegisters();
        }
        
        programCounter = nextPC;
        return true;
    }
    
    // Run the CPU with a program
    void run(vector<string>& program, int option) {
        executionOption = option;
        programCounter = 0;
        running = true;
        
        while (running && programCounter < (int)program.size()) {
            Instruction inst(program[programCounter]);
            
            // Skip empty lines
            if (inst.isEmpty()) {
                programCounter++;
                continue;
            }
            
            if (!executeInstruction(inst)) {
                running = false;
                break;
            }
        }
        
        if (running) {
            cout << "Program ended without HLT instruction." << endl;
            printRegisters();
        }
    }
};

// Class to handle program loading
class ProgramLoader {
public:
    // Load instructions from a file
    static vector<string> loadFromFile(string filename) {
        vector<string> instructions;
        ifstream file(filename.c_str());
        
        if (!file.is_open()) {
            cout << "Error: Cannot open file " << filename << endl;
            return instructions;
        }
        
        string line;
        while (getline(file, line)) {
            instructions.push_back(line);
        }
        
        file.close();
        return instructions;
    }
};

// Main function
int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <filename> <option>" << endl;
        cout << "option = 0: Run silently" << endl;
        cout << "option = 1: Run with instruction display" << endl;
        return 1;
    }
    
    string filename = argv[1];
    int option = StringUtils::stringToInt(argv[2]);
    
    // Validate option
    if (option != 0 && option != 1) {
        cout << "Error: Option must be 0 or 1" << endl;
        return 1;
    }
    
    // Load program from file
    vector<string> program = ProgramLoader::loadFromFile(filename);
    
    if (program.empty()) {
        cout << "Error: No instructions to execute" << endl;
        return 1;
    }
    
    // Create CPU and run program
    CPU cpu;
    cpu.run(program, option);
    
    return 0;
}

