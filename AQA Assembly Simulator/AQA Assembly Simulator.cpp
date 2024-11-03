#include <iostream>
#include <fstream>
#include <string>
#include <map>

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

int opcode(std::string(s)) {
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
    return opcodes[s];
}

unsigned int operand2(std::string operand, unsigned int *registers) {
    if (operand.substr(0, 1) == "#") {
        return std::stoi(operand.substr(1));
    }
    else if (operand.substr(0, 1) == "R") {
        return registers[std::stoi(operand.substr(1))];
    }
}

bool parse(std::string line, unsigned int *registers, unsigned int *memory, unsigned int *flag) {
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
        case STR:
            memory[std::stoi(instruction[2])] = registers[std::stoi(instruction[1].substr(1))];
        case ADD:
            registers[std::stoi(instruction[1].substr(1))] = registers[std::stoi(instruction[2].substr(1))] + operand2(instruction[3], registers);
        case SUB:
            registers[std::stoi(instruction[1].substr(1))] = registers[std::stoi(instruction[2].substr(1))] - operand2(instruction[3], registers);
        case MOV:
            registers[std::stoi(instruction[1].substr(1))] = operand2(instruction[2], registers);
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
        case ORR:
            registers[std::stoi(instruction[1].substr(1))] = registers[std::stoi(instruction[2].substr(1))] | operand2(instruction[3], registers);
        case EOR:
            registers[std::stoi(instruction[1].substr(1))] = registers[std::stoi(instruction[2].substr(1))] ^ operand2(instruction[3], registers);
        case MVN:
            registers[std::stoi(instruction[1].substr(1))] = ~(operand2(instruction[2], registers));
        case LSL:
            registers[std::stoi(instruction[1].substr(1))] = registers[std::stoi(instruction[2].substr(1))] << operand2(instruction[3], registers);
        case LSR:
            registers[std::stoi(instruction[1].substr(1))] = registers[std::stoi(instruction[2].substr(1))] >> operand2(instruction[3], registers);
        case HALT:
            *flag = 3;
    }
    
}

int main()
{
    std::cout << "AQA Assembly Interpreter\n";
    unsigned int registers[13]; 
    unsigned int memory[256];
    unsigned int flag;
    std::ifstream file("test.aqaasm");
    for (std::string line; std::getline(file, line);) {
        parse(line, registers, memory, &flag);
        if (flag == f_HALT) {
            break;
        }
    }
    return 0;
}
