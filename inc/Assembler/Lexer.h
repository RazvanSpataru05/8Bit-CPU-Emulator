#pragma once
#include <string>
#include <vector>
#include <cctype>
#include <functional>
#include <optional>

#include "InstructionDef.h"
#include "ISAEntry.h"
#include "Token.h"

using Handler = std::function<void()>;

class Lexer
{
public:
	Lexer(const std::string& sourceCode);

	void Tokenize();
	std::string GetTokenizedSourceCode() const;

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
	Token BuildNumericToken(const std::string& number, uint8_t prefix) const;

private:
	std::string m_sourceCode;
	uint32_t m_lineNumber;
	unsigned m_currentIndex;

	std::vector<Token> m_tokens;
	std::vector<std::string> m_errors;

	const std::vector<std::pair<std::function<bool(char)>, Handler>> m_handlers = {
		{[](char c) { return isalpha(c); },[this] {ConsumeWord(); }},
		{[](char c) {return isdigit(c); }, [this] {ConsumeNumber(); }},
		{[](char c) {return c == '\n'; }, [this] {ConsumeNewLine(); }},
		{[](char c) {return c == ';'; }, [this] {ConsumeComment(); }},
		{[](char c) {return c == ' ' || c == '\t'; }, [this] {m_currentIndex++; }}
	};
};

