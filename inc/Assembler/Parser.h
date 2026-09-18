#pragma once

#include "Assembler/Lexer.h"

#include <vector>

class Parser
{
public:
	Parser(const std::vector<Token>& tokens);

private:
	std::vector<Token> m_tokens;
};