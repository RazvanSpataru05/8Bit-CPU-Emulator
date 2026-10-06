#include "Assembler/Assembler.h"

Assembler::Assembler() :
	m_lexer{ Lexer() },
	m_parser{ Parser() }
{

}

bool Assembler::Assemble(std::string_view sourceCode)
{
	m_errors.clear();
	m_warnings.clear();

	RunLexer(sourceCode);
	if (m_lexer.LexerErrors())
	{
		const auto& lexerErrors = m_lexer.GetErrors();
		m_errors.insert(m_errors.end(), lexerErrors.begin(), lexerErrors.end());
		return false;
	}

	const auto& lexerWarnings = m_lexer.GetWarnings();
	m_warnings.insert(m_warnings.end(), lexerWarnings.begin(), lexerWarnings.end());

	RunParser();
	if (m_parser.ParserErrors())
	{
		const auto& parserErrors = m_parser.GetErrors();
		m_errors.insert(m_errors.end(), parserErrors.begin(), parserErrors.end());
		return false;
	}

	const auto& parserWarnings = m_parser.GetWarnings();
	m_warnings.insert(m_warnings.end(), parserWarnings.begin(), parserWarnings.end());

	SortErrors();
	SortWarnings();
	return m_errors.empty();
}

bool Assembler::HasErrors() const noexcept
{
	return !m_errors.empty();
}

std::span<const Statement> Assembler::GetStatements() const noexcept
{
	return m_parser.GetStatements();
}

std::span<const AssemblerError> Assembler::GetErrors() const noexcept
{
	return m_errors;
}

std::span<const AssemblerWarning> Assembler::GetWarnings() const noexcept
{
	return m_warnings;
}

void Assembler::SortErrors() noexcept
{
	std::sort(m_errors.begin(), m_errors.end());
}

void Assembler::SortWarnings() noexcept
{
	std::sort(m_warnings.begin(), m_warnings.end());
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
	m_parser.ParseTokens();
	m_parser.PrintStatements();
	m_parser.PrintLabels();
}
