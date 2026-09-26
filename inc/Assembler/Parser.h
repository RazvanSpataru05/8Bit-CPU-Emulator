#pragma once

#include "Assembler/Lexer.h"
#include "Assembler/LabelInfo.h"
#include "Assembler/Statement.h"

using namespace ISA;

using StatementHandler = std::function<void(const Token&)>;

class Parser
{
public:
	Parser() = default;
	Parser(std::span<const Token> tokens);

	Parser(Parser&&) = default;
	Parser& operator=(Parser&&) = default;

	void BuildSymbolTable();
	void ParseInstructions();

	bool ParserErrors() const noexcept;

	std::span<const Statement> GetStatements() const noexcept;

	void SetTokens(std::span<const Token> tokens);

	void ResetCurrentStatement();
	void PrintStatements() const noexcept;
	void PrintLabels() const noexcept;

private:
	Parser(const Parser&) = delete;
	Parser& operator=(const Parser&) = delete;

private:
	void HandleMnemonicToken(const Token& token);
	void HandleIdentifierToken(const Token& token);
	void HandleNewLineToken(const Token& token);

	void SkipOperandTokens(OperatorKind operatorKind);

	std::array<uint8_t, 2> ConsumeImm8();
	std::array<uint8_t, 2> ConsumeAddr16();
	std::array<uint8_t, 2> ConsumeReg();
	std::array<uint8_t, 2> ConsumeRegReg();

	void AddStatement();

	void ExpectEndOfStatement();
	void ExpectComma();
	void ExpectColon();

	void ConsumeLabel();

	bool IsLabelDefinition() const;

	/* Helpers */
	const Token& Peek() const;
	const Token& Next();
	uint32_t ConsumeNumber();
	uint8_t ConsumeSelector();

private:
	std::vector<Token> m_tokens;
	std::vector<AssemblerError> m_errors;
	std::unordered_map<std::string, LabelInfo> m_labels;

	uint16_t m_currentAddress{ 0x0000 };
	uint32_t m_lineNumber{};
	size_t m_pos{};

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