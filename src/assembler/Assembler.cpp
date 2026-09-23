#include "Assembler/Assembler.h"

Assembler::Assembler() :
	m_lexer{Lexer()},
	m_parser{Parser()}
{

}

bool Assembler::Assemble(std::string_view sourceCode)
{
	m_lexer.SetSourceCode(sourceCode);
	m_lexer.Tokenize();
	m_lexer.PrintTokenizedSourceCode();
	if (m_lexer.LexerErrors()) return false;

	m_parser.SetTokens(m_lexer.GetTokens());
	m_parser.ParseInstructions();
	m_parser.PrintStatements();
	if (m_parser.ParserErrors()) return false;

	return true;
}
