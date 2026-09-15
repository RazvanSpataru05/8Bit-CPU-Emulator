#pragma once
#include <array>
#include <string>
#include "InstructionDef.h"

class Dissasembler
{
public:
	Dissasembler();

	[[nodiscard]] const InstructionDef& GetInstructionDef(uint8_t index) const;

private:
	std::array<InstructionDef, 256> m_instructions;
};