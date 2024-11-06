#include <bits/stdc++.h>

// TODO implement branching, clean up code.

enum operations {
    LDR = 1,
    STR,
    ADD,
    SUB,
    MOV,
    CMP,
    B,
    BEQ,
    BNE,
    BGT,
    BLT,
    AND,
    ORR,
    EOR,
    MVN,
    LSL,
    LSR,
    HALT
};

enum flags {
    f_EQ = 0,
    f_GT,
    f_LT,
    f_HALT
};

std::string flagname(int flag) {
    std::string flagnames[4] = {"EQ", "GT", "LT", "HALT"};
    return flagnames[flag];
}

int opcode(std::string(s)) {
    std::string s_lower;
    for (int i = 0; i < s.length(); i++) {
        s_lower += std::tolower(s.at(i), std::locale(""));
    }
    std::map<std::string, int> opcodes = {
        {"ldr" ,  1},
        {"str" ,  2},
        {"add" ,  3},
        {"sub" ,  4},
        {"mov" ,  5},
        {"cmp" ,  6},
        {"b"   ,  7},
        {"beq" ,  8},
        {"bne" ,  9},
        {"bgt" , 10},
        {"blt" , 11},
        {"and" , 12},
        {"orr" , 13},
        {"eor" , 14},
        {"mvn" , 15},
        {"lsl" , 16},
        {"lsr" , 17},
        {"halt", 18},
    };
    return opcodes[s_lower];
}

unsigned int operand2(std::string operand, unsigned int *registers) {
    if (operand.substr(0, 1) == "#") {
        return std::stoi(operand.substr(1));
    }
    else { // if (operand.substr(0, 1) == "R")
        return registers[std::stoi(operand.substr(1,1))];
    }
}

void parse(std::string line, unsigned int *registers, unsigned int *memory, unsigned int *flag) {
    std::string instruction[4] = {"","","",""};
    std::string delimiter = " ";
    int token_counter = 0;
    while ((token_counter < 4) && (line.length() > 0)) {
        std::size_t delimiter_position = line.find(delimiter);
        instruction[token_counter] = line.substr(0, delimiter_position);
        line.erase(0, delimiter_position + delimiter.length());
        token_counter += 1;
    }
    switch (opcode(instruction[0])) {
        case LDR:
            registers[std::stoi(instruction[1].substr(1))] = memory[std::stoi(instruction[2])];
            break;
        case STR:
            memory[std::stoi(instruction[2])] = registers[std::stoi(instruction[1].substr(1))];
            break;
        case ADD:
            registers[std::stoi(instruction[1].substr(1))] = registers[std::stoi(instruction[2].substr(1))] + operand2(instruction[3], registers);
            break;
        case SUB:
            registers[std::stoi(instruction[1].substr(1))] = registers[std::stoi(instruction[2].substr(1))] - operand2(instruction[3], registers);
            break;
        case MOV:


            registers[std::stoi(instruction[1].substr(1))] = operand2(instruction[2], registers);

            break;
        case CMP:
            if (registers[std::stoi(instruction[1].substr(1))] == operand2(instruction[2], registers)) {
                *flag = f_EQ;
            }
            else if (registers[std::stoi(instruction[1].substr(1))] > operand2(instruction[2], registers)) {
                *flag = f_GT;
            }
            else {
                *flag = f_LT;
            }
            break;
        case B:
            // skip implementation for now;
        case BEQ:
            // skip implementation for now;;
        case BNE:
            // skip implementation for now;
        case BGT:
            // skip implementation for now;
        case BLT:
            // skip implementation for now;
        case AND:
            registers[std::stoi(instruction[1].substr(1))] = registers[std::stoi(instruction[2].substr(1))] & operand2(instruction[3], registers);
            break;
        case ORR:
            registers[std::stoi(instruction[1].substr(1))] = registers[std::stoi(instruction[2].substr(1))] | operand2(instruction[3], registers);
            break;
        case EOR:
            registers[std::stoi(instruction[1].substr(1))] = registers[std::stoi(instruction[2].substr(1))] ^ operand2(instruction[3], registers);
            break;
        case MVN:
            registers[std::stoi(instruction[1].substr(1))] = ~(operand2(instruction[2], registers));
            break;
        case LSL:
            registers[std::stoi(instruction[1].substr(1))] = registers[std::stoi(instruction[2].substr(1))] << operand2(instruction[3], registers);
            break;
        case LSR:
            registers[std::stoi(instruction[1].substr(1))] = registers[std::stoi(instruction[2].substr(1))] >> operand2(instruction[3], registers);
            break;
        case HALT:

            *flag = f_HALT;
    }

}

std::string m2s(unsigned int *m, unsigned int sz) {
    std::string s;
    for (int i = 0; i < sz; i++) {
        s += std::to_string(m[i]);
        s += " | ";
    }
    return s;
}

int main(int argc, char *argv[])
{
    std::cout << "AQA Assembly Interpreter\n";
    unsigned int registers[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned int memory[256];
    for(int i = 0; i < 256; i++) {
        memory[i] = 0;
    }
    unsigned int flag = 0;
    std::ifstream file(argv[1]);
    for (std::string line; std::getline(file, line);) {
        std::cout << line << "\n";
        parse(line, registers, memory, &flag);
        std::cout << "Registers: " << m2s(registers, 13) << "\n";
        std::cout << "Memory: " << m2s(memory, 256) << "\n";
        std::cout << "Flag: " << flagname(flag) << "\n";
        if (flag == f_HALT) {
            std::cout << "halting due to instruction" << "\n";
            return 0;
        }
    }
    std::cout << "halting due to EOF" << "\n";
    return 0;
}
