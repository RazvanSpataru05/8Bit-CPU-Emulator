#pragma once

#include <cstdint>

struct InstructionTableInfo
{
	const char* title = nullptr;
	const char* tableId = nullptr;
	uint8_t firstInstruction{};
	uint8_t lastInstruction{};


	constexpr InstructionTableInfo(const char* t, const char* id, uint8_t first, uint8_t last) :
		title{ t }, tableId{ id }, firstInstruction{ first }, lastInstruction{ last }
	{

	}
};