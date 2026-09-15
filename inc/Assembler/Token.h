#pragma once

enum class TokenType
{
	MNEMONIC,
	REGISTER,
	NUMBER,
	LABEL_DEF,
	LABEL_REF,
	NEWLINE,
	END
};

struct Token
{
	TokenType type{};
	std::string value{};
	uint32_t line{};
	Token() = default;
	Token(TokenType t, std::string_view v, uint32_t l) :
		type{ t }, value{ v }, line{ l } {
	}
};