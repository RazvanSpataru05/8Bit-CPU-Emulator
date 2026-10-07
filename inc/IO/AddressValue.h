#pragma once

#include <cstdint>

struct AddressValue
{
	uint16_t address{};
	uint8_t value{};

	AddressValue(uint16_t a, uint8_t v) :
		address{ a }, value{ v }
	{
	}
};