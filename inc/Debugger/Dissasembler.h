#pragma once

#include "Assembler/InstructionDef.h"

#include <array>
#include <string>

class Dissasembler
{
public:
	Dissasembler();

	[[nodiscard]] const InstructionDef& GetInstructionDef(size_t index) const;

private:
	std::array<InstructionDef, 256> m_instructions;
};