#pragma once

#include <filesystem>
#include <vector>
#include <fstream>

namespace ProgramLoader
{
	std::vector<uint8_t> ParseHexValues(const std::filesystem::path& path, bool& hasData);
};