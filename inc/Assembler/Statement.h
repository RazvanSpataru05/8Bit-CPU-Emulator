#pragma once

#include "Assembler/ISAEntry.h"

#include <array>
#include <cstdint>

struct Statement
{
	uint8_t opcode;
	std::array<uint8_t, 2> operands;
	ISA::ISAEntry* ISAEntry; // temporary, for print debugging

	Statement() = default;
	Statement(uint8_t oc, const std::array<uint8_t, 2>& op, ISA::ISAEntry* entry) :
		opcode{ oc }, operands{ op }, ISAEntry{ entry } {
	}
};