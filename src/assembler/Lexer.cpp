#include "Assembler/Lexer.h"

Lexer::Lexer(std::string_view sourceCode) :
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
		const unsigned char currentChar = m_sourceCode[m_currentIndex];
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
		}
	}
	m_tokens.emplace_back(TokenType::END_OF_FILE, "END_OF_FILE", ++m_lineNumber);
}

bool Lexer::LexerErrors() const noexcept
{
	return !m_errors.empty();
}

std::string Lexer::GetTokenizedSourceCode() const noexcept
{
	std::string tokenizedSourceCode;
	for (const auto& token : m_tokens)
	{
		tokenizedSourceCode += Utils::TokenTypeToString(token) + ": " + token.value + '\n';
	}
	return tokenizedSourceCode;
}

void Lexer::PrintTokenizedSourceCode() const noexcept
{
	std::cout << m_tokens.size() << std::endl << std::endl;
	std::cout << GetTokenizedSourceCode();
}

void Lexer::SetSourceCode(std::string_view sourceCode)
{
	m_sourceCode = sourceCode;
}

const std::vector<Error>& Lexer::GetErrors() const
{
	return m_errors;
}

std::span<const Token> Lexer::GetTokens() const
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

void Lexer::ConsumeComment()
{
	while (m_currentIndex < m_sourceCode.size() && m_sourceCode[m_currentIndex] != '\n')
	{
		++m_currentIndex;
	}
}

void Lexer::ConsumeNewLine()
{
	ConsumeSymbol(TokenType::NEW_LINE, "\n");
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

Token Lexer::BuildToken(std::string_view word)
{
	const std::string upperWord = Utils::ToUpper(word);

	if (nameToSelector.contains(upperWord)) return { TokenType::REGISTER, word, m_lineNumber };
	if (ISA::IsMnemonic(upperWord)) return { TokenType::MNEMONIC, word, m_lineNumber };

	if (Utils::StartsLikeNumber(word))
	{
		if (!Utils::IsNumber(word)) ReportError("Invalid number");
		return { TokenType::NUMBER, word, m_lineNumber };
	}
	return { TokenType::IDENTIFIER, word, m_lineNumber };
}
