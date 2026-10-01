#include "Assembler/Lexer.h"

Lexer::Lexer(std::string_view sourceCode) :
	m_sourceCode{ sourceCode },
	m_lineNumber{ 1u },
	m_columnNumber{ 1u },
	m_currentIndex{ 0 }
{
}

void Lexer::Tokenize()
{
	m_tokens.clear();
	m_errors.clear();
	m_warnings.clear();
	m_currentIndex = 0;
	m_lineNumber = 1u;
	m_columnNumber = 1u;

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
			AddError(std::format("Error at line {}, column {}: unexpected character '{}'.",
				m_lineNumber, m_columnNumber, currentChar), m_columnNumber);
		}
	}
	m_tokens.emplace_back(TokenType::END_OF_FILE, "END_OF_FILE", ++m_lineNumber, 1u);
}

bool Lexer::LexerErrors() const noexcept
{
	return !m_errors.empty();
}

std::span<const AssemblerError> Lexer::GetErrors() const noexcept
{
	return m_errors;
}

std::span<const AssemblerWarning> Lexer::GetWarnings() const noexcept
{
	return m_warnings;
}

void Lexer::PrintTokenizedSourceCode() const noexcept
{
	for (const auto& token : m_tokens)
	{
		std::cout << "Token Type: " << Utils::TokenTypeToString(token) << std::endl;
		std::cout << "Token Value: " << token.value << std::endl;
		std::cout << "Token Line: " << token.line << std::endl;
		std::cout << "Token Column: " << token.column << std::endl << std::endl;
 	}
}

void Lexer::SetSourceCode(std::string_view sourceCode)
{
	m_sourceCode = sourceCode;
}

std::span<const Token> Lexer::GetTokens() const noexcept
{
	return m_tokens;
}

void Lexer::ConsumeWord()
{
	std::string word{};
	while (m_currentIndex < m_sourceCode.size() &&
		(isalnum(m_sourceCode[m_currentIndex]) || m_sourceCode[m_currentIndex] == '_'))
	{
		word += m_sourceCode[m_currentIndex];
		++m_columnNumber;
		++m_currentIndex;
	}
	
	const Token& token = BuildToken(word);
	m_tokens.emplace_back(token);
	CheckMixedCase(token);
}

void Lexer::ConsumeWhiteSpace()
{
	while (m_currentIndex < m_sourceCode.size() &&
		(m_sourceCode[m_currentIndex] == ' ' ||
			m_sourceCode[m_currentIndex] == '\t'))
	{
		++m_currentIndex;
		++m_columnNumber;
	}
}

void Lexer::ConsumeComment()
{
	while (m_currentIndex < m_sourceCode.size() && m_sourceCode[m_currentIndex] != '\n')
	{
		++m_currentIndex;
		++m_columnNumber;
	}
}

void Lexer::ConsumeNewLine()
{
	ConsumeSymbol(TokenType::NEW_LINE, "\n");
	++m_lineNumber;
	m_columnNumber = 1u;
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
	m_tokens.emplace_back(tokenType, symbol, m_lineNumber, m_columnNumber);
	++m_currentIndex;
	++m_columnNumber;
}

void Lexer::CheckMixedCase(const Token& token)
{
	if (token.type != TokenType::REGISTER && token.type != TokenType::MNEMONIC) return;

	const std::string upperWord = Utils::ToUpper(token.value);

	if (token.value == upperWord) return;

	const std::string kind = token.type == TokenType::MNEMONIC ? "mnemonic" : "register";

	AddWarning(std::format("Warning at line {}, column {}: {} '{}' is not uppercase. Consider {}.",
		token.line, token.column, kind, token.value, upperWord), token.column);
}

void Lexer::AddError(std::string_view message, uint32_t column)
{
	m_errors.emplace_back(AssemblerStage::LEXER, m_lineNumber, column, message);
}

void Lexer::AddWarning(std::string_view message, uint32_t column)
{
	m_warnings.emplace_back(AssemblerStage::LEXER, m_lineNumber, column, message);
}

Token Lexer::BuildToken(std::string_view word)
{
	const std::string upperWord = Utils::ToUpper(word);
	const uint32_t wordColumnStart = static_cast<uint32_t>(m_columnNumber - word.size());

	if (nameToSelector.contains(upperWord)) return { TokenType::REGISTER, word, m_lineNumber, wordColumnStart };
	if (ISA::IsMnemonic(upperWord)) return { TokenType::MNEMONIC, word, m_lineNumber, wordColumnStart };

	if (Utils::StartsLikeNumber(word))
	{
		if (Utils::IsNumber(word))
		{
			return { TokenType::NUMBER, word, m_lineNumber, wordColumnStart };
		}
		else
		{
			AddError(std::format("Error at line {}, column {}: '{}' is not a valid number.",
				m_lineNumber, wordColumnStart, word), wordColumnStart);
		}
	}
	return { TokenType::IDENTIFIER, word, m_lineNumber, wordColumnStart };
}
