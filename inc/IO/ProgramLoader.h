#pragma once

#include <filesystem>
#include <vector>
#include <fstream>

class ProgramLoader
{
public:
	ProgramLoader() = default;

	std::vector<uint8_t> ParseHexValues(const std::filesystem::path& path);

private:

};