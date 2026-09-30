#pragma once

#include <cstdint>

struct ParsedNumber
{
	uint32_t value{};
	bool outOfRange{ false };

	ParsedNumber(uint32_t v, bool o) :
		value{ v }, outOfRange{ o }
	{

	}
};