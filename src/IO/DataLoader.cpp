#include "IO/DataLoader.h"

std::vector<uint8_t> DataLoader::ParseHexValues(const std::filesystem::path& path, bool& hasData)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		throw std::runtime_error("Error: could not open file " + path.string());
	}

	std::vector<uint8_t> values;
	unsigned hexValue{};
	while (file >> std::hex >> hexValue)
	{
		values.emplace_back(static_cast<uint8_t>(hexValue));
		if (hexValue != 0x00)
		{
			hasData = true;
		}
	}
	file.close();
	return values;
}

std::vector<AddressValue> DataLoader::ParseStatements(std::span<const Statement> statements)
{
	std::vector<AddressValue> values;

	for (const auto& statement : statements)
	{
		uint16_t address = statement.address;
		Logger::AddInfoMessage(std::format("Address: {}", address));

		// this is an instruction
		if (statement.opcode.has_value()) 
		{
			values.push_back({ address++, statement.opcode.value() });	
		}

		for (size_t index = 0; index < statement.operatorCount; ++index)
		{
			values.push_back({ address++, statement.operands[index] });
		}


		if (statement.opcode.has_value())
		{
			Logger::AddInfoMessage("This is a regular instruction\n");
		}
		else
		{
			Logger::AddInfoMessage("This is a DW statement\n");
		}
	}
	return values.size() < std::numeric_limits<uint16_t>::max() ? values : std::vector<AddressValue>();
}
