#pragma once

#include "Utils/CharacterUtils.h"

#include <string>
#include <algorithm>

namespace Utils
{
	std::string ToUpper(std::string_view word);

	void CheckBase(std::string_view prefix, uint8_t& base);

	bool HasPrefix(std::string_view word);
	bool StartsLikeNumber(std::string_view word);
	bool IsNumber(std::string_view word);
}