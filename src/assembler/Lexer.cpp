#include "Assembler/Lexer.h"

Lexer::Lexer(const std::string& sourceCode) :
	m_sourceCode{ sourceCode },
	m_lineNumber{ 1u },
	m_currentIndex{ 0 }
{
	std::cout << "String View Source Code Size: " << m_sourceCode.size() << std::endl;
}

void Lexer::Tokenize()
{
	if (m_sourceCode.empty()) return;

	m_tokens.clear();
	m_errors.clear();

	while (m_currentIndex < m_sourceCode.size())
	{
		std::cout << "Hello from while block" << std::endl;
		const char currentChar = m_sourceCode[m_currentIndex];
		std::cout << "char = [" << currentChar << "]\n";
		std::cout << "current index: " << m_currentIndex << '\n';
		auto it = std::find_if(m_handlers.begin(), m_handlers.end(), [currentChar](const auto& pair)
			{
				return pair.first(currentChar);
			});
		if (it != m_handlers.end())
		{
			std::cout << "Handler found " << std::endl;
			it->second();
		}
		else
		{
			m_errors.emplace_back(m_lineNumber, std::string_view("Error: Unknown character " + currentChar));
			std::cout << "hello from error recovery block" << std::endl;
			ErrorRecovery();
		}
	}

	m_tokens.emplace_back(TokenType::END, "HLT", m_lineNumber);
}

std::string Lexer::GetTokenizedSourceCode() const
{
	std::string tokenizedSourceCode;
	for (const auto& token : m_tokens)
	{
		tokenizedSourceCode += GetTokenType(token) + ": " + token.value + '\n';
	}
	return tokenizedSourceCode;
}

void Lexer::PrintTokenizedSourceCode() const noexcept
{
	std::cout << m_tokens.size() << std::endl << std::endl;
	const std::string tokenizedSourceCode = GetTokenizedSourceCode();
	std::cout << tokenizedSourceCode;
}

const std::vector<Error>& Lexer::GetErrors() const
{
	return m_errors;
}

const std::vector<Token>& Lexer::GetTokens() const
{
	return m_tokens;
}

void Lexer::ConsumeWord()
{
	std::string word;
	while (m_currentIndex < m_sourceCode.size() &&
		(isalnum(m_sourceCode[m_currentIndex]) || m_sourceCode[m_currentIndex] == '_'))
	{
		word += m_sourceCode[m_currentIndex];
		++m_currentIndex;
	}
	m_tokens.emplace_back(BuildToken(word));
}

void Lexer::ConsumeWhiteSpace()
{
	while (m_currentIndex < m_sourceCode.size() &&
		(m_sourceCode[m_currentIndex] == ' ' ||
		m_sourceCode[m_currentIndex] == '\t'))
	{
		++m_currentIndex;
	}
}

void Lexer::CheckBase(const std::string& prefix, uint8_t& base)
{
	if (prefix == "0x") base = 16u;
	else if (prefix == "0b") base = 2u;
}

bool Lexer::IsPrefix() const
{
	if (m_currentIndex + 1 < m_sourceCode.size() &&
		(m_sourceCode[m_currentIndex] == '0' &&
			(m_sourceCode[m_currentIndex + 1] == 'b' || m_sourceCode[m_currentIndex + 1] == 'x')))
	{
		return true;
	}
	return false;
}

void Lexer::ConsumeNumber()
{
	std::string number;
	std::string prefix;
	uint8_t base = 10u;

	if (IsPrefix())
	{
		prefix = m_sourceCode[m_currentIndex] + m_sourceCode[++m_currentIndex];
		CheckBase(prefix, base);
	}

	auto isValidDigit = [](char c, uint8_t base) -> bool
		{
			if (base == 2) return c == '0' || c == '1';
			if (base == 16) return isxdigit(c);
			return isdigit(c);
		};

	while (m_currentIndex < m_sourceCode.size() && isValidDigit(m_sourceCode[m_currentIndex], base))
	{
		number += m_sourceCode[m_currentIndex];
		++m_currentIndex;
	}

	if (const auto error = CheckLexicalNumericError(base, number); error.has_value())
	{
		m_errors.emplace_back(m_lineNumber, error.value());
		return;
	}

	if (m_currentIndex < m_sourceCode.size() && isalnum(m_sourceCode[m_currentIndex]))
	{
		m_errors.emplace_back(m_lineNumber, "Line " + std::to_string(m_lineNumber) +
			": Invalid digit " + m_sourceCode[m_currentIndex] + " for base " + std::to_string(base) + ".");
		ErrorRecovery();
		return;
	}
}

std::optional<std::string> Lexer::CheckLexicalNumericError(uint8_t base, const std::string& number) const
{
	// Base is diff than 10 (number has prefix), but number is empty
	if (base != 10 && number.empty())
	{
		return "Line " + std::to_string(m_lineNumber) +
			": No number present after base " + std::to_string(base) + ".";
	}
	return std::nullopt;
}

void Lexer::ConsumeComment()
{
	while (m_currentIndex < m_sourceCode.size() && m_sourceCode[m_currentIndex] != '\n')
	{
		++m_currentIndex;
	}
}

void Lexer::ErrorRecovery()
{
	while (m_currentIndex < m_sourceCode.size()
		&& m_sourceCode[m_currentIndex] != ' '
		&& m_sourceCode[m_currentIndex] != '\t'
		&& m_sourceCode[m_currentIndex] != '\n')
	{
		if (m_sourceCode[m_currentIndex] == '\n')
		{
			++m_lineNumber;
		}
		++m_currentIndex;
	}
}

std::string Lexer::GetTokenType(const Token& token) const
{
	switch (token.type)
	{
	case TokenType::REGISTER:	return "REGISTER";
	case TokenType::MNEMONIC:	return "MNEMONIC";
	case TokenType::LABEL_DEF:	return "LABEL_DEF";
	case TokenType::LABEL_REF:	return "LABEL_REF";
	case TokenType::NUMBER:		return "NUMBER";
	case TokenType::END:		return "END";
	default:					return "ERROR";
	}
}

Token Lexer::BuildToken(const std::string& word)
{
	Token token;
	token.line = m_lineNumber;
	token.value = word;
	//std::transform(word.begin(), word.end(), word.begin(), ::toupper);

	auto it = nameToSelector.find(word);
	if (it != nameToSelector.end())
	{
		token.type = TokenType::REGISTER;
		return token;
	}

	if (IsMnemonic(word))
	{
		token.type = TokenType::MNEMONIC;
		return token;
	}

	if (m_sourceCode[m_currentIndex] == ':')
	{
		token.type = TokenType::LABEL_DEF;
		++m_currentIndex;
		return token;
	}
	return token;
}
