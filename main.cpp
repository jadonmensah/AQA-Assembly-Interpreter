// AQA assembly interpreter - last updated 21/11/24 by Jadon Mensah
// Notes: TODO fix branching

#include <bits/stdc++.h>
#define SZ_INST_MEM 1024
#define SZ_DATA_MEM 1024 // max 1048576 addressable locations
#define NUM_REGS 13 // maximum of 13 addressable locations

using namespace std;

string bindump(int x) {
    string res = "";
    for(int i = 0; i < 32; i++) {
        res += ((x >> i) & 1) + '0';
    }
    return string(res.rbegin(), res.rend());
}

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

char mnemonic2opcode(string s)
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

string memdisplay(int16_t *m, unsigned int sz)
{
    string s;
    for (int i = 0; i < sz; i++) {
        s += to_string(m[i]);
        s += " | ";
    }
    return s;
}

void splitline(string line, string *instruction) {
    stringstream ss(line);
    string word;
    int counter;
    for (int i = 0; (ss >> word) && (i < 4); i++) {
        instruction[i] = word;
    }
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
    cout << line << "\n";
    splitline(line, instruction);

    // Assuming inputs are consistent with specification
    int operation = mnemonic2opcode(instruction[0]) << 28;
    int register1;
    int register2;
    int registerval1;
    int registerval2;
    int immediate1;
    int immediate2;
    int memory_ref;


    if ((operation >> 28) != B) {
        register1 = stoi(instruction[1].substr(1)) << 24;
        register2 = stoi(instruction[2].substr(1)) << 20;
        registerval1 = (stoi(instruction[2].substr(1)) | 0x00FFFFF0);
        registerval2 = (stoi(instruction[3].substr(1)) | 0x000FFFF0);
        immediate1 = stoi(instruction[2].substr(1));
        immediate2 = stoi(instruction[3].substr(1));
    }



    try {
        memory_ref = stoi(instruction[2]) | 0x00AA0000;
    } catch (...) {
        memory_ref = 0x00AA0000;
    }
    int label = label_table[instruction[1]] | 0x01110000;
    switch (mnemonic2opcode(instruction[0]))
    {

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
    cout << bindump(operation) << "\n";
    return operation;
}

int run(int inst, int ip, int16_t *regs, int16_t *data_memory, char *flag)
{
    // modify regs/memory/flag as needed
    // set ip increment
    // select instruction by opcode & modify regs/memory/flag as necessary

    int opcode = (inst >> 28) & 0xF;
    cout << bindump(opcode) << "\n";
    int register1 = (inst >> 24) & 0xF;
    int register2 = (inst >> 20) & 0xF;
    int registerval = inst & 0xF;
    int16_t immediate = inst & 0xFFFF;
    int16_t memory_ref = inst & 0xFFFF;
    int16_t jump_dest = inst & 0xFFFF;
    int16_t operand2;
    !((inst >> 16) & 0xF) ? operand2 = immediate : operand2 = regs[registerval];

    switch(opcode)
    {

    case LDR:
        regs[register1] = data_memory[memory_ref];
        break;
    case STR:
        data_memory[memory_ref] = regs[register1];
        break;
    case ADD:
        regs[register1] = regs[register2] + operand2;
        break;
    case SUB:
        regs[register1] = regs[register2] - operand2;
        break;
    case MOV:
        regs[register1] = operand2;
        break;
    case CMP:
        *flag = (regs[register1] == operand2) & 0x1
             | ((regs[register1] < operand2) << 1) & 0b10;
        break;
    case B:
        return jump_dest - ip;
        break;
    case BEQ:
        if (*flag & 0x1) return jump_dest - ip;
        break;
    case BLT:
        if (*flag & 0b10) return jump_dest - ip;
        break;
    case AND:
        regs[register1] = regs[register2] & operand2;
        break;
    case ORR:
        regs[register1] = regs[register2] | operand2;
        cout << bindump(regs[register1]) << "\n";
        cout << bindump(regs[register2]) << "\n";
        cout << bindump(operand2) << "\n";
        break;
    case EOR:
        regs[register1] = regs[register2] ^ operand2;
        break;
    case MVN:
        regs[register1] = ~operand2;
        break;
    case LSL:
        regs[register1] = regs[register2] << operand2;
        break;
    case LSR:
        regs[register1] = regs[register2] >> operand2;
        break;
    case HALT:
        // this case should not be reachable
        cout << "[debug] HALT case reached in run()\n";

    }
    return 1;
}



int main(int argc, char *argv[])
{
    // initialise various things we'll need later
    int instruction_memory[SZ_INST_MEM] = {0};
    int16_t data_memory[SZ_DATA_MEM] = {0};
    int16_t regs[NUM_REGS] = {0};
    map<string, int> label_table;
    int instruction_counter, length;
    char flag = 0; // LSB - equal flag, 2nd least significant bit - LT flag

    // iterate through lines in file specified by first argument
    std::ifstream file(argv[1]);

     cout << "Parsing instructions...\n";
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

     cout << "Executing...\n";
    // once file has been parsed, execute from instruction_memory[0] until HALT
    int ip = 0; // instruction pointer
    while(instruction_memory[ip] != HALT)
    {
        int step = run(instruction_memory[ip], ip, regs, data_memory, &flag);
        // display data memory, registers, flag and ip
        cout << "Instruction:  " << bindump(instruction_memory[ip]) << "\n";
        cout << "Registers: " << memdisplay(regs, NUM_REGS) << "\n";
        cout << "Memory: "
             << memdisplay(data_memory, SZ_DATA_MEM) << "\n";
        cout << "Equality flag: " << (flag & 1) << "\n";
        cout << "Less than flag: " << ((flag & 2) >> 1) << "\n";
        ip += step;

    }
    return 0;
}
