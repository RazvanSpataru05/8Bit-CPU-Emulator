#pragma once

#include "Assembler/ISAEntry.h"

#include <array>
#include <cstdint>
#include <optional>

struct Statement
{
	std::optional<uint8_t> opcode = std::nullopt;
	uint8_t operatorCount{};
	std::array<uint8_t, 2> operands{};
	const ISA::ISAEntry* ISAEntry = nullptr; // temporary, for print debugging

	Statement() = default;
};