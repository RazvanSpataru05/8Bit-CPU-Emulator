#pragma once

#include "Assembler/InstructionDef.h"
#include "Assembler/ISAEntry.h"
#include "Assembler/Token.h"
#include "Assembler/Error.h"

#include "Utils/CharacterUtils.h"
#include "Utils/StringUtils.h"

#include <iostream>
#include <vector>
#include <functional>

using Handler = std::function<void()>;

class Lexer
{
public:
	Lexer(std::string_view sourceCode);

	Lexer(Lexer&&) = default;
	Lexer& operator=(Lexer&&) = default;

	void Tokenize();
	std::string GetTokenizedSourceCode() const;
	void PrintTokenizedSourceCode() const noexcept;

	const std::vector<Error>& GetErrors() const;
	const std::vector<Token>& GetTokens() const;

private:
	void ConsumeWord();
	void ConsumeWhiteSpace();
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

	void ReportError(std::string_view error);
	std::string GetTokenType(const Token& token) const;

	Token BuildToken(std::string_view word);

private:
	Lexer(const Lexer&) = delete;
	Lexer& operator=(const Lexer&) = delete;

private:
	std::string m_sourceCode;
	uint32_t m_lineNumber;
	size_t m_currentIndex;

	std::vector<Token> m_tokens;
	std::vector<Error> m_errors;

	std::vector<std::pair<std::function<bool(unsigned char)>, Handler>> m_handlers =
	{
		{[](unsigned char c) {return isalnum(c);}, [this] {ConsumeWord();}},
		{[](unsigned char c) {return c == ';';}, [this] {ConsumeComment();} },
		{[](unsigned char c) {return c == ' ' || c == '\t';}, [this] {ConsumeWhiteSpace();}},
		{[](unsigned char c) {return c == '\n';}, [this] {ConsumeNewLine();}},
		{[](unsigned char c) {return c == ':';}, [this] {ConsumeColon();}},
		{[](unsigned char c) {return c == ',';}, [this] {ConsumeComma();}},
		{ [](unsigned char c) {return c == '(';}, [this] {ConsumeLeftParanthesis();} },
		{ [](unsigned char c) {return c == ')';}, [this] {ConsumeRightParanthesis();} },
		{ [](unsigned char c) {return c == '[';}, [this] {ConsumeLeftBracket();} },
		{ [](unsigned char c) {return c == ']';}, [this] {ConsumeRightBracket();} }
	};
};