#pragma once

#include "Assembler/AssemblerStage.h"

#include <string>

struct AssemblerWarning
{
	AssemblerStage stage;
	uint32_t line;
	uint32_t column;
	std::string message;

	AssemblerWarning(AssemblerStage s, uint32_t l, uint32_t c, std::string_view m)
		: stage{ s }, line{ l }, column{ c }, message{ m }
	{
	}

	constexpr bool operator<(const AssemblerWarning& rhs) const
	{
		if (line == rhs.line)
		{
			return column < rhs.column;
		}
		return line < rhs.line;
	}
};