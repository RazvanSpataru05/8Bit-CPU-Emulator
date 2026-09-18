#pragma once

#include "Assembler/InstructionDef.h"
#include "Assembler/ISAEntry.h"
#include "Assembler/Token.h"
#include "Assembler/Error.h"

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <functional>
#include <optional>
#include <algorithm>

using Handler = std::function<void()>;

class Lexer
{
public:
	Lexer(const std::string& sourceCode);

	void Tokenize();
	std::string GetTokenizedSourceCode() const;
	void PrintTokenizedSourceCode() const noexcept;

	const std::vector<Error>& GetErrors() const;
	const std::vector<Token>& GetTokens() const;

private:
	void ConsumeWord();
	void ConsumeWhiteSpace();
	void ConsumeNumber();
	void ConsumeComment();
	void ConsumeNewLine();
	void ConsumeColon();
	void ConsumeComma();
	void ConsumeLeftParanthesis();
	void ConsumeRightParanthesis();
	void ConsumeLeftBracket();
	void ConsumeRightBracket();

	// Helper for other tokens
	void ConsumeSymbol(TokenType tokenType, std::string_view symbol);

	void ReportError(const std::string& error);
	void ErrorRecovery();
	std::string GetTokenType(const Token& token) const;

	void CheckBase(const std::string& prefix, uint8_t& base);
	bool IsPrefix() const;
	std::optional<std::string> CheckLexicalNumericError(uint8_t base, const std::string& number) const;

	Token BuildToken(const std::string& word);

private:
	std::string m_sourceCode;
	uint32_t m_lineNumber;
	size_t m_currentIndex;

	std::vector<Token> m_tokens;
	std::vector<Error> m_errors;

	std::vector<std::pair<std::function<bool(char)>, Handler>> m_handlers =
	{
		{[](char c) {return isalnum(c);}, [this] {ConsumeWord();}},
		{[](char c) {return c == ';';}, [this] {ConsumeComment();} },
		{[](char c) {return c == ' ' || c == '\t';}, [this] {ConsumeWhiteSpace();}},
		{[](char c) {return c == '\n';}, [this] {ConsumeNewLine();}},
		{[](char c) {return c == ':';}, [this] {ConsumeColon();}},
		{[](char c) {return c == ',';}, [this] {ConsumeComma();}},
		{ [](char c) {return c == '(';}, [this] {ConsumeLeftParanthesis();} },
		{ [](char c) {return c == ')';}, [this] {ConsumeRightParanthesis();} },
		{ [](char c) {return c == '[';}, [this] {ConsumeLeftBracket();} },
		{ [](char c) {return c == ']';}, [this] {ConsumeRightBracket();} }
	};
};