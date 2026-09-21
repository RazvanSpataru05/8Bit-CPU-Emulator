#pragma once

#include "Assembler/Statement.h"

#include <filesystem>
#include <vector>
#include <fstream>
#include <span>

namespace DataLoader
{
	std::vector<uint8_t> ParseHexValues(const std::filesystem::path& path, bool& hasData);
	std::vector<uint8_t> ParseStatements(std::span<const Statement> statements);
};