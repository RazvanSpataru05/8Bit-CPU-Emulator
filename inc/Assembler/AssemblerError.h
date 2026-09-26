#pragma once

#include <cstdint>
#include <string>

enum class Severity : uint8_t
{
	WARNING = 1,
	ERROR
};

enum class Stage : uint8_t
{
	LEXER = 1,
	PARSER,
	LOADER
};

struct AssemblerError
{
	Severity severity;
	Stage stage;
	uint32_t line{};
	uint32_t column{};
	std::string message;

	AssemblerError(Severity sev, Stage st, uint32_t l, uint32_t c, std::string_view m) :
		severity{ sev }, stage{ st }, line{ l }, column{ c }, message{ m }
	{
	}
};