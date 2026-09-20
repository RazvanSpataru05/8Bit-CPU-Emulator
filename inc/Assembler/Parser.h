#pragma once

#include "Assembler/Lexer.h"
#include "Assembler/Statement.h"

#include <vector>

using StatementHandler = std::function<void(const Token&)>;

class Parser
{
public:
	Parser(const std::vector<Token>& tokens);

	Parser(Parser&&) = default;
	Parser& operator=(Parser&&) = default;

	void Parse();

	void BuildStatement();
	void AddStatement();

	void ResetCurrentStatement();
	void PrintStatements() const;

private:
	Parser(const Parser&) = delete;
	Parser& operator=(const Parser&) = delete;

private:
	void HandleMnemonicToken(const Token& token);
	void HandleIdentifierToken(const Token& token);
	void HandleNewLineToken(const Token& token);

private:
	std::vector<Token> m_tokens;

	uint32_t m_lineNumber;
	size_t m_currentTokenIndex;

	Statement m_currentStatement;
	std::vector<Statement> m_statements;

	const std::vector<std::pair<std::function<bool(const Token&)>, StatementHandler>> m_handlers =
	{
		{[](const Token& token) {return token.type == TokenType::MNEMONIC;},
			[this](const Token& token) {HandleMnemonicToken(token);}},

		{[](const Token& token) {return token.type == TokenType::IDENTIFIER;},
			[this](const Token& token) {HandleIdentifierToken(token);}},

		{[](const Token& token) {return token.type == TokenType::NEW_LINE;},
			[this](const Token& token) {HandleNewLineToken(token);}},
	};
};