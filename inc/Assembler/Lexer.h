#pragma once

#include "Assembler/InstructionDef.h"
#include "Assembler/ISAEntry.h"
#include "Assembler/Token.h"

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <functional>
#include <optional>

using Handler = std::function<void()>;

class Lexer
{
public:
	Lexer(const std::string& sourceCode);

	void Tokenize();
	std::string GetTokenizedSourceCode() const;
	void PrintTokenizedSourceCode() const noexcept;

	const std::vector<std::string>& GetErrors() const;
	const std::vector<Token>& GetTokens() const;

private:
	void ConsumeWord();
	void ConsumeNumber();
	void ConsumeNewLine();
	void ConsumeComment();

	void ErrorRecovery();
	std::string GetTokenType(const Token& token) const;

	void CheckBase(const std::string& prefix, uint8_t& base);
	bool IsPrefix() const;
	std::optional<std::string> CheckLexicalNumericError(uint8_t base, const std::string& number) const;

	Token BuildToken(const std::string& word);

private:
	std::string_view m_sourceCode;
	uint32_t m_lineNumber;
	size_t m_currentIndex;

	std::vector<Token> m_tokens;
	std::vector<std::string> m_errors;

	std::vector<std::pair<std::function<bool(unsigned char)>, Handler>> m_handlers =
	{
		{[](unsigned char c) {return std::isalpha(c);}, [this] {ConsumeWord();}}
	};
};