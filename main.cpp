// AQA assembly interpreter - last updated 09/11/24 by Jadon Mensah
// Notes: this is a non-functional outline of a bytecode vm interpreter

#include <bits/stdc++.h>
#define SZ_INST_MEM sizeof(int[1024])
#define SZ_DATA_MEM sizeof(int[1024])
#define NUM_REGS 12

using namespace std;

int bytecode(string line, map<string, int> label_table)
{
    /* 1 operation = 32 bits for convenience, but this is memory inefficient.
     * From least significant bit (0) to most significant (31):
     * [0,  3] - opcode (NB: BNE, BGT not needed; so 16 instructions, not 18)
     *                  (preprocessing is needed to convert BNEs and BGTs)
     *         - 0000 HALT
     *         - 0001 LDR
     *         - 0010 STR
     *         - 0011 ADD
     *         - 0100 SUB
     *         - 0101 MOV
     *         - 0110 CMP
     *         - 0111 B
     *         - 1000 BEQ
     *         - 1001 BLT
     *         - 1010 AND
     *         - 1011 ORR
     *         - 1100 EOR
     *         - 1101 MVN
     *         - 1110 LSL
     *         - 1111 LSR
     * [4, 31] - instruction-dependent operand bits - either:
     *         - <register>, <memory ref>
     *         - <register>, <immediate>
     *         - <register>, <register>
     *         - <register>, <register>, <immediate>
     *         - <register>, <register>, <register>
     *         - <label>
     * <register> - 4 bits, only registers 0 through 12 (0b1100) used
     * <memory ref> - 24 bits; highest 4 bits = 1111 => 20 significant bits
     * <immediate> - 20 bits; highest 4 bits = 1110 => 16 significant bits
     * <label> - 28 bits; highest 4 bits = 1101 => 24 significant bits
     */
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
            instruction_memory[ip] != o_HALT;
            ip += run(instruction_memory[ip], ip, regs, data_memory, &flag);)
    {
        // display both instruction and data memories, registers, flag and ip
    }
    free(instruction_memory);
    free(data_memory);
    return 0;
}
