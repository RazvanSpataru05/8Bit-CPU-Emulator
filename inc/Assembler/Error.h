#pragma once

#include <cstdint>
#include <string>

struct Error
{
	uint32_t lineNumber{};
	std::string errorInfo;

	Error(uint32_t l, std::string_view e) : lineNumber{ l }, errorInfo{ e } {};
};