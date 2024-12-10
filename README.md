# AQA Assembly Interpreter
Interpreter for a small subset of the ARM assembly language, as used in AQA A-level Computer Science exams (see [the specification](#specification)).

## Compiling
`g++ .\main.cpp -o aai.exe` (tested: gcc version 8.1.0, MinGW-W64 on Windows 11)

## Usage
`aai.exe filename` interprets the assembly code in the file named `filename`.

After each instruction is executed, flag, memory and register values are printed.

## Specification
The specification can be found at https://filestore.aqa.org.uk/resources/computing/AQA-75162-75172-ALI.PDF

## Implementation choices
Things that the specification does not specify, or where I've chosen not to follow the specification:
- 16-bit data word size
- 32-bit instruction word size
- Instructions stored in separate memory from program data
- Instruction binary format - see block comment in `bytecode()` function.
- BGT and BNE instructions not implemented so that opcodes fit in 4 bits.

## Limitations
- Programs are limited to 2^16 unique labels due to the design of the instruction binary format
- Programs are limited to 2^16 addressable data memory locations due to the design of the instruction binary format

## Planned changes and enhancements
(in order of increasing difficulty)
- Quiet mode (don't print memory/registers/flags on every instruction)
- Change instruction binary format so that:
  - word size is 32 bits
  - BGT & BNE can be implemented
  - Instructions and data live in the same memory
  - Input/Output extension instructions can be implemented
- Transpile to C or some other language
