// AQA assembly interpreter - last updated 11/11/24 by Jadon Mensah
// Notes: implemented bytecode function. TODO implement run function

#include <bits/stdc++.h>
#define SZ_INST_MEM sizeof(int[1024])
#define SZ_DATA_MEM sizeof(int16_t[1024]) // max 1048576 addressable locations
#define NUM_REGS 12 // maximum of 13 addressable locations

using namespace std;

enum opcodes
{
    HALT = 0,
    LDR,
    STR,
    ADD,
    SUB,
    MOV,
    CMP,
    B,
    BEQ,
    BLT,
    AND,
    ORR,
    EOR,
    MVN,
    LSL,
    LSR,
};

char mnemonic2opcode(string(s))
{
    string s_lower;
    for (int i = 0; i < s.length(); i++)
    {
        s_lower += std::tolower(s.at(i), std::locale(""));
    }
    std::map<std::string, int> opcodes_local =
    {
        {"ldr",    LDR},
        {"str",    STR},
        {"add",    ADD},
        {"sub",    SUB},
        {"mov",    MOV},
        {"cmp",    CMP},
        {"b",        B},
        {"beq",    BEQ},
        {"blt",    BLT},
        {"and",    AND},
        {"orr",    ORR},
        {"eor",    EOR},
        {"mvn",    MVN},
        {"lsl",    LSL},
        {"lsr",    LSR},
        {"halt",  HALT},
    };
    return opcodes_local[s_lower];
}

int bytecode(string line, map<string, int> label_table)
{
    /* 1 operation = 32 bits for convenience, but this is memory inefficient.
     * From most significant bit (0) to least significant (31):
     * [0,  3] - opcode (NB: BNE, BGT not needed; so 16 instructions, not 18)
     *                  (preprocessing is needed to convert BNEs and BGTs)
     * [4, 31] - instruction-dependent operand bits - either:
     *         - <register1>, <memory ref>
     *         - <register1>, <immediate1>
     *         - <register1>, <register-val1>
     *         - <register1>, <register2>, <immediate2>
     *         - <register1>, <register2>, <register-val2>
     *         - <label>
     * <register> - 4 bits, only registers 0 through 12 (0b1100) used
     * <memory ref> - 24 bits; highest 8 bits = 10101010 => 16 significant bits
     * <immediate1> - 24 bits; highest 8 bits = 00000000 => 16 significant bits
     * <immediate2> - 20 bits; highest 4 bits = 0000 => 16 significant bits
     * <register-val1> - 24 bits; highest 20 bits = 0xFFFFF => 4 significant bits
     * <register-val2> - 20 bits; highest 16 bits = 0xFFFF => 4 significant bits
     * <label> - 28 bits; highest 12 bits = 0x111 => 16 significant bits
     */
    string instruction[4] = {"00","00","00","00"};
    string delimiter = " ";
    int token_counter = 0;
    while ((token_counter < 4) && (line.length() > 0))
    {
        std::size_t delimiter_position = line.find(delimiter);
        instruction[token_counter] = line.substr(0, delimiter_position);
        line.erase(0, delimiter_position + delimiter.length());
        token_counter += 1;
    }
    int operation;
    switch (mnemonic2opcode(instruction[0]))
    {
    default: // Assuming inputs are consistent with specification
        operation = mnemonic2opcode(instruction[0]) << 28;
        int register1 = stoi(instruction[1].substr(1)) << 24;
        int register2 = stoi(instruction[2].substr(1)) << 20;
        int registerval1 = (stoi(instruction[2].substr(1)) | 0x00FFFFF0);
        int registerval2 = (stoi(instruction[3].substr(1)) | 0x000FFFF0);
        int immediate1 = stoi(instruction[2].substr(1));
        int immediate2 = stoi(instruction[3].substr(1));
        int memory_ref = stoi(instruction[2]) | 0x00AA0000;
        int label = label_table[instruction[1]] | 0x01110000;
    case LDR: case STR:
        operation |= register1 | memory_ref;
        break;
    case ADD: case SUB: case AND: case ORR: case EOR: case LSL: case LSR:
        operation |= register1 | register2;
        if (instruction[3].at(0) == 'R') {
            operation |= registerval2;
        } else {
            operation |= immediate2;
        }
        break;
    case MOV: case CMP: case MVN:
        operation |= register1;
        if (instruction[2].at(0) == 'R') {
            operation |= registerval1;
        } else {
            operation |= immediate1;
        }
        break;
    case B: case BEQ: case BLT:
        operation |= label;
        break;
    }
    return operation;
}

int main(int argc, char *argv[])
{
    // initialise various things we'll need later
    int *instruction_memory = malloc(SZ_INST_MEM);
    int16_t *data_memory = malloc(SZ_DATA_MEM);
    memset(data_memory, 0, SZ_DATA_MEM);
    int16_t regs[NUM_REGS];
    map<string, int> label_table;
    int instruction_counter, length;

    // iterate through lines in file specified by first argument
    std::ifstream file(argv[1]);
    for (std::string line; std::getline(file, line);)
    {
        length = line.length();
        // if line is instruction, write bytecode representation to memory
        if (line.at(length - 1) != ':')
        {
            instruction_memory[instruction_counter] = bytecode(line, label_table);
            instruction_counter++;
        }
        else
        {
            // line is a label. store address of label so we can resolve later
            label_table[line.substr(0, length - 1)] = instruction_counter + 1;
        }
    }
    // once file has been parsed, execute from instruction_memory[0] until HALT
    for(int ip = 0; // instruction pointer
            instruction_memory[ip] != HALT;
            ip += run(instruction_memory[ip], ip, regs, data_memory, &flag);)
    {
        // display both instruction and data memories, registers, flag and ip
    }
    free(instruction_memory);
    free(data_memory);
    return 0;
}
