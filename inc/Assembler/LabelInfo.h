#pragma once

#include <cstdint>

struct LabelInfo
{
	uint16_t address{};
	uint32_t lineDeclaration{};
	bool used{ false };

	LabelInfo() = default;
	LabelInfo(uint16_t a, uint32_t l) : address{ a }, lineDeclaration{ l } {}
};