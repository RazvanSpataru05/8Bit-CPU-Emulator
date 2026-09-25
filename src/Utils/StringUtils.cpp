#include "Utils/StringUtils.h"

std::string Utils::ToUpper(std::string_view word)
{
	std::string upper{ word };
	std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
	return upper;
}

std::string Utils::ToLower(std::string_view word)
{
	std::string lower{ word };
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	return lower;
}

void Utils::CheckBase(std::string_view prefix, uint8_t& base)
{
	if (prefix == "0b" || prefix == "0B") base = 2u;
	else if (prefix == "0x" || prefix == "0X") base = 16u;
}

bool Utils::HasPrefix(std::string_view word)
{
	if (word.size() < 2) return false;
	return word[0] == '0' && (word[1] == 'b' || word[1] == 'B' || word[1] == 'x' || word[1] == 'X');
}

bool Utils::StartsLikeNumber(std::string_view word)
{
	if (word.empty()) return false;
	return Utils::HasPrefix(word) || isdigit(word[0]);
}

bool Utils::IsNumber(std::string_view word)
{
	std::string prefix;
	uint8_t base = 10u;
	size_t startingPosition{};

	if (HasPrefix(word))
	{
		startingPosition += 2;
		prefix = word.substr(0, 2);
		CheckBase(prefix, base);
	}

	return std::all_of(word.begin() + startingPosition, word.end(), [base](unsigned char c) {
		return Utils::IsValidDigit(c, base);
		});
}

uint32_t Utils::ParseNumber(const std::string& word)
{
	uint8_t base = 10u;
	if (HasPrefix(word))
	{
		const std::string prefix = word.substr(0, 2);
		CheckBase(prefix, base);
	}
	std::cout << "Out Parse number" << std::endl;
	return static_cast<uint32_t>(std::stoi(word, nullptr, base));
}

std::string_view Utils::OperatorKindToString(ISA::OperatorKind operatorKind)
{
	switch (operatorKind)
	{
	case ISA::OperatorKind::NONE:					return "None";
	case ISA::OperatorKind::IMM_8:					return "Immediate 8-bit value";
	case ISA::OperatorKind::ADDR_16:				return "16-bit Address";
	case ISA::OperatorKind::REG:					return "Registry or Selector Code";
	case ISA::OperatorKind::REG_REG:				return "Two Registries";
	}
	return "Error";
}

const char* Utils::FlagToString(bool value)
{
	return value ? "True" : "False";
}
