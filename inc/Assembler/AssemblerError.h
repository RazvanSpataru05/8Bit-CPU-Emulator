#pragma once

#include "Assembler/AssemblerStage.h"

#include <string>

struct AssemblerError
{
	AssemblerStage stage;
	uint32_t line{};
	uint32_t column{};
	std::string message;

	AssemblerError(AssemblerStage st, uint32_t l, uint32_t c, std::string_view m) :
		stage{ st }, line{ l }, column{ c }, message{ m }
	{
	}

	constexpr bool operator<(const AssemblerError& rhs) const
	{
		if (line == rhs.line)
		{
			return column < rhs.column;
		}
		return line < rhs.line;
	}
};