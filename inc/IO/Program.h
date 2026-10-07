#pragma once

#include "IO/AddressValue.h"

#include <span>
#include <vector>

struct Program
{
	uint16_t startAddress{};
	std::vector<AddressValue> addrValues;

	Program(uint16_t startAddr, std::span<const AddressValue> addrVal) :
		startAddress{ startAddr }
	{
		addrValues.assign(addrVal.begin(), addrVal.end());
	}
};