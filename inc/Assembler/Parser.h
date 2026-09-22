#pragma once

#include "Assembler/Lexer.h"
#include "Assembler/Statement.h"

#include <vector>

using StatementHandler = std::function<void(const Token&)>;

class Parser
{
public:
	Parser() = default;
	Parser(const std::vector<Token>& tokens);

	Parser(Parser&&) = default;
	Parser& operator=(Parser&&) = default;

	std::span<const Statement> GetStatements() const noexcept;

	void ParseInstructions();

	void ResetCurrentStatement();
	void PrintStatements() const;

private:
	Parser(const Parser&) = delete;
	Parser& operator=(const Parser&) = delete;

private:
	void HandleMnemonicToken(const Token& token);
	void HandleIdentifierToken(const Token& token);
	void HandleNewLineToken(const Token& token);

	std::array<uint8_t, 2> ConsumeImm8();
	std::array<uint8_t, 2> ConsumeAddr16();
	std::array<uint8_t, 2> ConsumeReg();
	std::array<uint8_t, 2> ConsumeRegReg();

	void AddStatement();
	void ExpectEndOfStatement();
	void ExpectComma();

	/* Helpers */
	const Token& Next();
	uint32_t ConsumeNumber();
	uint8_t ConsumeSelector();

private:
	const std::vector<Token> m_tokens;

	uint32_t m_lineNumber;
	size_t m_pos;

	Statement m_currentStatement;
	std::vector<Statement> m_statements;

	std::vector<std::pair<std::function<bool(const Token&)>, StatementHandler>> m_handlers =
	{
		{[](const Token& token) {return token.type == TokenType::MNEMONIC;},
			[this](const Token& token) {HandleMnemonicToken(token);}},

		{[](const Token& token) {return token.type == TokenType::IDENTIFIER;},
			[this](const Token& token) {HandleIdentifierToken(token);}},

		{[](const Token& token) {return token.type == TokenType::NEW_LINE;},
			[this](const Token& token) {HandleNewLineToken(token);}},
	};
};