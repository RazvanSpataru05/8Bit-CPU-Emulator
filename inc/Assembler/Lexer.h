#pragma once

#include "Assembler/InstructionDef.h"
#include "Assembler/ISAEntry.h"
#include "Assembler/Token.h"
#include "Assembler/AssemblerError.h"
#include "Assembler/AssemblerWarning.h"

#include "Utils/CharacterUtils.h"
#include "Utils/StringUtils.h"

#include <iostream>
#include <vector>
#include <functional>

using TokenHandler = std::function<void()>;

class Lexer
{
public:
	Lexer() = default;
	Lexer(std::string_view sourceCode);

	Lexer(Lexer&&) = default;
	Lexer& operator=(Lexer&&) = default;

	void Tokenize();

	bool LexerErrors() const noexcept;

	std::span<const Token>				GetTokens()		const noexcept;
	std::span<const AssemblerError>		GetErrors()		const noexcept;
	std::span<const AssemblerWarning>	GetWarnings()	const noexcept;

	void PrintTokenizedSourceCode() const noexcept;

	void SetSourceCode(std::string_view sourceCode);

private:
	void ConsumeWord();
	void ConsumeWhiteSpace();
	void ConsumeComment();
	void ConsumeNewLine();
	void ConsumeColon();
	void ConsumeComma();
	void ConsumeDot();
	void ConsumeLeftParanthesis();
	void ConsumeRightParanthesis();
	void ConsumeLeftBracket();
	void ConsumeRightBracket();

	// Helper for other tokens
	void ConsumeSymbol(TokenType tokenType, std::string_view symbol);

	void CheckMixedCase(const Token& token);

	void AddError(std::string_view message, uint32_t column);
	void AddWarning(std::string_view message, uint32_t column);

	Token BuildToken(std::string_view word);

private:
	Lexer(const Lexer&) = delete;
	Lexer& operator=(const Lexer&) = delete;

private:
	std::string m_sourceCode;
	uint32_t m_lineNumber{};
	uint32_t m_columnNumber{};
	size_t m_currentIndex{};

	std::vector<Token> m_tokens;
	std::vector<AssemblerError> m_errors;
	std::vector<AssemblerWarning> m_warnings;

	const std::vector<std::pair<std::function<bool(unsigned char)>, TokenHandler>> m_handlers =
	{
		{[](unsigned char c) {return isalnum(c);}, [this] {ConsumeWord();}},

		{[](unsigned char c) {return c == ';';}, [this] {ConsumeComment();} },

		{[](unsigned char c) {return c == ' ' || c == '\t';}, [this] {ConsumeWhiteSpace();}},

		{[](unsigned char c) {return c == '\n';}, [this] {ConsumeNewLine();}},

		{[](unsigned char c) {return c == ':';}, [this] {ConsumeColon();}},

		{[](unsigned char c) {return c == ',';}, [this] {ConsumeComma();}},

		{[](unsigned char c) {return c == '.';}, [this] {ConsumeDot();}},

		{[](unsigned char c) {return c == '(';}, [this] {ConsumeLeftParanthesis();} },

		{[](unsigned char c) {return c == ')';}, [this] {ConsumeRightParanthesis();} },

		{[](unsigned char c) {return c == '[';}, [this] {ConsumeLeftBracket();} },

		{[](unsigned char c) {return c == ']';}, [this] {ConsumeRightBracket();} }
	};
};