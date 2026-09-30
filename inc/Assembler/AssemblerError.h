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

	const bool operator<(const AssemblerError& other) const
	{
		if (line == other.line)
		{
			return column < other.column;
		}
		return line < other.line;
	}
};