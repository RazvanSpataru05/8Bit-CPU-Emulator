#include "Debugger/Dissasembler.h"

Dissasembler::Dissasembler()
{
	std::fill(m_instructions.begin(), m_instructions.end(), InstructionDef("???", 1u));

	// No operation instruction
	m_instructions[0x00] = { "NOP",		1u };

	// Load/Store instructions
	m_instructions[0x01] = { "LDA_IM",  2u };
	m_instructions[0x02] = { "LDA_DIR", 3u };
	m_instructions[0x03] = { "LDA_REG", 2u };
	m_instructions[0x04] = { "STA_DIR", 3u };
	m_instructions[0x05] = { "LDB_IM",  2u };
	m_instructions[0x06] = { "LDB_DIR", 3u };
	m_instructions[0x07] = { "LDB_REG", 2u };
	m_instructions[0x08] = { "STB_DIR", 3u };
	m_instructions[0x09] = { "LDC_IM",  2u };
	m_instructions[0x0A] = { "LDC_DIR", 3u };
	m_instructions[0x0B] = { "LDC_REG", 2u };
	m_instructions[0x0C] = { "STC_DIR", 3u };
	m_instructions[0x0D] = { "LDD_IM",  2u };
	m_instructions[0x0E] = { "LDD_DIR", 3u };
	m_instructions[0x0F] = { "LDD_REG", 2u };
	m_instructions[0x10] = { "STD_DIR", 3u };
	m_instructions[0x11] = { "LDA_IND", 2u };
	m_instructions[0x12] = { "STA_IND", 2u };
	m_instructions[0x13] = { "LDW_IM",	2u };
	m_instructions[0x14] = { "INCW",	1u };


	// Arithmetic instructions
	m_instructions[0x20] = { "ADD_IM",  2u };
	m_instructions[0x21] = { "ADD_REG", 2u };
	m_instructions[0x22] = { "ADC_IM",  2u };
	m_instructions[0x23] = { "ADC_REG", 2u };
	m_instructions[0x24] = { "SUB_IM",  2u };
	m_instructions[0x25] = { "SUB_REG", 2u };
	m_instructions[0x26] = { "SBC_IM",  2u };
	m_instructions[0x27] = { "SBC_REG", 2u };
	m_instructions[0x28] = { "INC",     2u };
	m_instructions[0x29] = { "DEC",     2u };
	m_instructions[0x2A] = { "MUL_IM",  2u };
	m_instructions[0x2B] = { "MUL_REG", 2u };

	// Logical instructions
	m_instructions[0x30] = { "AND_IM",  2u };
	m_instructions[0x31] = { "AND_REG", 2u };
	m_instructions[0x32] = { "OR_IM",   2u };
	m_instructions[0x33] = { "OR_REG",  2u };
	m_instructions[0x34] = { "XOR_IM",  2u };
	m_instructions[0x35] = { "XOR_REG", 2u };
	m_instructions[0x36] = { "NOT",     2u };
	m_instructions[0x37] = { "SHL",     2u };
	m_instructions[0x38] = { "SHR",     2u };

	// Compare instructions
	m_instructions[0x40] = { "CMP_IM",  2u };
	m_instructions[0x41] = { "CMP_REG", 2u };

	// Jump instructions
	m_instructions[0x50] = { "JMP",     3u };
	m_instructions[0x51] = { "JZ",      3u };
	m_instructions[0x52] = { "JNZ",     3u };
	m_instructions[0x53] = { "JC",      3u };
	m_instructions[0x54] = { "JNC",     3u };
	m_instructions[0x55] = { "JN",      3u };
	m_instructions[0x56] = { "JNN",     3u };
	m_instructions[0x57] = { "JO",      3u };
	m_instructions[0x58] = { "JNO",     3u };

	// Stack instructions
	m_instructions[0x60] = { "PUSH",    2u };
	m_instructions[0x61] = { "POP",     2u };
	m_instructions[0x62] = { "CALL",    3u };
	m_instructions[0x63] = { "RET",     1u };

	// Misc
	m_instructions[0x70] = { "MOV",     3u };
	m_instructions[0xFF] = { "HLT",     1u };
}

const InstructionDef& Dissasembler::GetInstructionDef(size_t index) const
{
	return m_instructions[index];
}
