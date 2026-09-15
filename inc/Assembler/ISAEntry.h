#pragma once
#include <cstdint>
#include <string>

struct ISAEntry
{
    const char* mnemonic;
    uint8_t opcode;
    uint8_t size;
    const char* description;
};

static const ISAEntry isaTable[] = {

    /* Load / Store instructions */
    {"LDA_IM", 0x01, 2, "Loads immediate value into register A"},
    {"LDA_DIR", 0x02, 3, "Loads value at immediate memory address into register A"},
    {"LDA_REG", 0x03, 2, "Loads value into register A"},
    {"STA_DIR", 0x04, 3, "Writes value of register A at memory address specified"},
    {"LDB_IM", 0x05, 2, "Loads immediate value into register B" },
    {"LDB_DIR", 0x06, 3, "Loads value at immediate memory address into register B"},
    {"LDB_REG", 0x07, 2, "Loads value from another register into register B"},
    {"STB_DIR", 0x08, 3, "Writes value of register B at memory address specified"},
    {"LDC_IM", 0x09, 2, "Loads immediate value into register C" },
    {"LDC_DIR", 0x0A, 3, "Loads value at immediate memory address into register C"},
    {"LDC_REG", 0x0B, 2, "Loads value from another register into register C"},
    {"STC_DIR", 0x0C, 3, "Writes value of register C at memory address specified"},
    {"LDD_IM", 0x0D, 2, "Loads immediate value into register D" },
    {"LDD_DIR", 0x0E, 3, "Loads value at immediate memory address into register D"},
    {"LDD_REG", 0x0F, 2, "Loads value from another register into register D"},
    {"STD_DIR", 0x10, 3, "Writes value of register D at memory address specified"},
    {"LDA_IND", 0x11, 2, "Loads into register A the value from 16-bit register BC"},
    {"STA_IND", 0x12, 2, "Writes into memory address of the BC register the value from register A "},
    {"LDW_IM", 0x13, 3, "Loads a word (16 bits) into registers B and C. The first 8 bits are loaded into register B and the following 8 into register C"},
    {"INCW", 0x14, 1, "Forms the 16-bit BC register and increments it"},

    /* Arithmetic operations */
    {"ADD_IM", 0x20, 2, "Adds immediate value to register A. Updates flags"},
    {"ADD_REG", 0x21, 2, "Adds another register value to register A. Updates flags"},
    {"ADC_IM", 0x22, 2, "Adds value at specified memory address to register A. Updates flags"},
    {"ADC_REG", 0x23, 2, "Adds another register value to register A and checks for carry flag. Updates flags"},
    {"SUB_IM", 0x24, 2, "Subtracts immediate value from register A. Updates flags"},
    {"SUB_REG", 0x25, 2, "Subtracts another register value from register A. Updates flags"},
    {"SBC_IM", 0x26, 2, "Subtracts immediate value from register A and checks for carry flag. Updates flags"},
    {"SBC_REG", 0x27, 2, "Subtracts another register value from register A and checks for carry flag. Updates flags"},
    {"INC", 0x28, 2, "Increments specified register by 1. Updates flags"},
    {"DEC", 0x29, 2, "Decrements specified register by 1. Updates flags"},
    {"MUL_IM", 0x2A, 2, "Multiplies register A by immediate value. Updates flags"},
    {"MUL_REG", 0x2B, 2, "Multiplies another register value by register A. Updates flags"},

    /* Logical operations */
    {"AND_IM", 0x30, 2, "Applies logical AND between immediate value and register A. Updates flags"},
    {"AND_REG", 0x31, 2, "Applies logical AND between specified register and register A. Updates flags"},
    {"OR_IM", 0x32, 2, "Applies logical OR between immediate value and register A. Updates flags"},
    {"OR_REG", 0x33, 2, "Applies logical OR between specified register and register A. Updates flags"},
    {"XOR_IM", 0x34, 2, "Applies logical XOR between immediate value and register A. Updates flags"},
    {"XOR_REG", 0x35, 2, "Applies logical XOR between specified register and register A. Updates flags"},
    {"NOT", 0x36, 2, "Applies the NOT operator on a specified register. Updates flags"},
    {"SHL", 0x37, 2, "Shifts a specified register to the left by 1 position. Updates flags"},
    {"SHR", 0x38, 2, "Shifts a specified register to the right by 1 position. Updates flags"},

    /* Compare operations */
    {"CMP_IM", 0x40, 2, "Compares immediate value to register A. Updates flags"},
    {"CMP_REG", 0x41, 2, "Compares specified register value to register A. Updates flags"},

    /* Jump/Branch operations */
    {"JMP", 0x50, 3, "Computes 16-bit address. Assigns the address to PC"},
    {"JZ", 0x51, 3, "Computes 16-bit address. Assigns the address to PC if Zero Flag is true"},
    {"JNZ", 0x52, 3, "Computes 16-bit address. Assigns the address to PC if Zero Flag is NOT true"},
    {"JC", 0x53, 3, "Computes 16-bit address. Assigns the address to PC if Carry Flag is true"},
    {"JNC", 0x54, 3, "Computes 16-bit address. Assigns the address to PC if Carry Flag is NOT true"},
    {"JN", 0x55, 3, "Computes 16-bit address. Assigns the address to PC if Negative Flag is true"},
    {"JNN", 0x56, 3, "Computes 16-bit address. Assigns the address to PC if Negative Flag is NOT true"},
    {"JO", 0x57, 3, "Computes 16-bit address. Assigns the address to PC if Overflow Flag is true"},
    {"JNO", 0x58, 3, "Computes 16-bit address. Assigns the address to PC if Overflow Flag is NOT true"},

    /* Stack operations */
    {"PUSH", 0x60, 2, "Assigns the value of a specified register to the memory address the SP points. Decrements SP"},
    {"POP", 0x61, 2, "Assigns to a specified register the value of the memory addres the SP points. Increments SP"},
    {"CALL", 0x62, 3, "Jumps to subroutines at given address, saving return address on the stack"},
    {"RET", 0x63, 1, "Assigns the return address to PC"},

    /* Misc */
    {"NOP", 0x00, 1, "No operation"},
    {"MOV", 0x70, 3, "Copies the value of a register to another register"},
    {"HLT", 0XFF, 1, "Stops the execution of the program"}
};

const ISAEntry* GetISATable(int& outSize);

bool IsMnemonic(const std::string& word);