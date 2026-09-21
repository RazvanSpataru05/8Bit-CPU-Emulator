#pragma once

#include "Assembler/ISAEntry.h"

#include <array>
#include <cstdint>

struct Statement
{
	uint8_t opcode = 0x00;
	size_t operatorCount = 0u;
	std::array<uint8_t, 2> operands{};
	const ISA::ISAEntry* ISAEntry = nullptr; // temporary, for print debugging

	Statement() = default;
};