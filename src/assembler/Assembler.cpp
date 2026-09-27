#include "Assembler/Assembler.h"

Assembler::Assembler() :
	m_lexer{Lexer()},
	m_parser{Parser()}
{

}

bool Assembler::Assemble(std::string_view sourceCode)
{
	m_errors.clear();

	RunLexer(sourceCode);
	if (m_lexer.LexerErrors())
	{
		const auto& lexerErrors = m_lexer.GetErrors();
		m_errors.insert(m_errors.end(), lexerErrors.begin(), lexerErrors.end());
		return false;
	}

	RunParser();
	if (m_parser.ParserErrors())
	{
		const auto& parserErrors = m_parser.GetErrors();
		m_errors.insert(m_errors.end(), parserErrors.begin(), parserErrors.end());
		return false;
	}

	SortErrors();
	return m_errors.empty();
}

std::span<const Statement> Assembler::GetStatements() const noexcept
{
	return m_parser.GetStatements();
}

std::span<const AssemblerError> Assembler::GetErrors() const noexcept
{
	return m_errors;
}

void Assembler::SortErrors() noexcept
{
	std::sort(m_errors.begin(), m_errors.end());
}

void Assembler::RunLexer(std::string_view sourceCode)
{
	m_lexer.SetSourceCode(sourceCode);
	m_lexer.Tokenize();
	m_lexer.PrintTokenizedSourceCode();
}

void Assembler::RunParser()
{
	m_parser.SetTokens(m_lexer.GetTokens());
	m_parser.ParseInstructions();
	m_parser.PrintStatements();
	m_parser.PrintLabels();
}
