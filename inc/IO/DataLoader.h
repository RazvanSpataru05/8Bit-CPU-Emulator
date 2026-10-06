#pragma once

#include "Assembler/Statement.h"

#include "Utils/Logger.h"

#include <filesystem>
#include <vector>
#include <fstream>
#include <span>

struct AddressValue
{
	uint16_t address{};
	uint8_t value{};

	AddressValue(uint16_t a, uint8_t v) :
		address{ a }, value{ v }
	{
	}
};

namespace DataLoader
{
	std::vector<uint8_t> ParseHexValues(const std::filesystem::path& path, bool& hasData);
	std::vector<AddressValue> ParseStatements(std::span<const Statement> statements);
};