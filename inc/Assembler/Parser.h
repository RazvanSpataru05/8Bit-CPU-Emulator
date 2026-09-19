#pragma once

#include "Assembler/Lexer.h"

#include <vector>

class Parser
{
public:
	Parser(const std::vector<Token>& tokens);

	Parser(Parser&&) = default;
	Parser& operator=(Parser&&) = default;

private:
	Parser(const Parser&) = delete;
	Parser& operator=(const Parser&) = delete;

private:
	std::vector<Token> m_tokens;
};