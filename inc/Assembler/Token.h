#pragma once

enum class TokenType
{
	NONE=0,

	MNEMONIC,
	REGISTER, // A-D or a-d
	NUMBER,
	NEW_LINE,
	IDENTIFIER,

	LEFT_PARAN,
	RIGHT_PARAN,
	LEFT_BRACKET,
	RIGHT_BRACKET,

	COLON,
	COMMA,

	END
};

struct Token
{
	TokenType type{};
	std::string value{};
	uint32_t line{};

	Token(TokenType t, std::string_view v, uint32_t l) :
		type{ t }, value{ v }, line{ l } {
	}
};