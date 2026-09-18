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
	m_tokens.clear();
	m_errors.clear();
	m_currentIndex = 0;
	m_lineNumber = 1u;

	while (m_currentIndex < m_sourceCode.size())
	{
		const char currentChar = m_sourceCode[m_currentIndex];
		auto it = std::find_if(m_handlers.begin(), m_handlers.end(), [currentChar](const auto& handler)
			{
				return handler.first(currentChar);
			});

		if (it != m_handlers.end())
		{
			it->second();
		}
		else
		{
			ReportError("Unknown character " + currentChar);
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
	std::cout << GetTokenizedSourceCode();
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
	m_tokens.push_back(BuildToken(word));
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

void Lexer::CheckBase(std::string_view prefix, uint8_t& base) const
{
	if (prefix == "0b" || prefix == "0B") base = 2u;
	else if (prefix == "0x" || prefix == "0X") base = 16u;
}

bool Lexer::HasPrefix(std::string_view word) const
{
	if (word.size() < 2) return false;
	return word[0] == '0' && (word[1] == 'b' || word[1] == 'B' || word[1] == 'x' || word[1] == 'X');
}

void Lexer::ConsumeComment()
{
	while (m_currentIndex < m_sourceCode.size() && m_sourceCode[m_currentIndex] != '\n')
	{
		++m_currentIndex;
	}
}

void Lexer::ConsumeNewLine()
{
	++m_currentIndex;
	++m_lineNumber;
}

void Lexer::ConsumeColon()
{
	ConsumeSymbol(TokenType::COLON, ":");
}

void Lexer::ConsumeComma()
{
	ConsumeSymbol(TokenType::COMMA, ",");
}

void Lexer::ConsumeLeftParanthesis()
{
	ConsumeSymbol(TokenType::LEFT_PARAN, "(");
}

void Lexer::ConsumeRightParanthesis()
{
	ConsumeSymbol(TokenType::RIGHT_PARAN, ")");
}

void Lexer::ConsumeLeftBracket()
{
	ConsumeSymbol(TokenType::LEFT_BRACKET, "[");
}

void Lexer::ConsumeRightBracket()
{
	ConsumeSymbol(TokenType::LEFT_BRACKET, "]");
}

void Lexer::ConsumeSymbol(TokenType tokenType, std::string_view symbol)
{
	m_tokens.emplace_back(tokenType, symbol, m_lineNumber);
	++m_currentIndex;
}

void Lexer::ReportError(std::string_view error)
{
	std::cout << "Invalid number";
	m_errors.emplace_back(m_lineNumber, error);
}

void Lexer::ErrorRecovery()
{
	while (m_currentIndex < m_sourceCode.size() &&
		(m_sourceCode[m_currentIndex] == ' ' ||
			m_sourceCode[m_currentIndex] == '\t' ||
			m_sourceCode[m_currentIndex] == '\n'))
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
	case TokenType::REGISTER:		return "REGISTER";
	case TokenType::MNEMONIC:		return "MNEMONIC";
	case TokenType::IDENTIFIER:		return "IDENTIFIER";
	case TokenType::NUMBER:			return "NUMBER";

	case TokenType::LEFT_PARAN:		return "LEFT_PARAN";
	case TokenType::RIGHT_PARAN:	return "RIGHT_PARAN";
	case TokenType::LEFT_BRACKET:	return "LEFT_BRACKET";
	case TokenType::RIGHT_BRACKET:	return "RIGHT_BRACKET";

	case TokenType::COLON:			return "COLON";
	case TokenType::COMMA:			return "COMMA";

	case TokenType::END:			return "END";

	default:						return "ERROR";
	}
}

bool Lexer::IsNumber(std::string_view word) const
{
	std::string prefix;
	uint8_t base = 10u;
	size_t startingPosition{};

	if (HasPrefix(word))
	{
		startingPosition += 2;
		prefix = word.substr(0, 2);
		CheckBase(prefix, base);
	}

	return std::all_of(word.begin() + startingPosition, word.end(), [base](char c) {
		return Utils::IsValidDigit(c, base);
		});
}

Token Lexer::BuildToken(std::string_view word)
{
	std::string upperWord{ word };
	std::transform(upperWord.begin(), upperWord.end(), upperWord.begin(), ::toupper);

	auto it = nameToSelector.find(upperWord);
	if (it != nameToSelector.end())
	{
		return { TokenType::REGISTER, word, m_lineNumber };
	}
	else if (ISA::IsMnemonic(upperWord))
	{
		return { TokenType::MNEMONIC, word, m_lineNumber };
	}
	else
	{
		if (IsNumber(word))
		{
			return { TokenType::NUMBER, word, m_lineNumber };
		}
		else ReportError("Invalid number");
	}
	return { TokenType::IDENTIFIER, word, m_lineNumber };
}
