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
	if (m_lexer.LexerErrors()) return false;

	m_parser.SetTokens(m_lexer.GetTokens());
	m_parser.ParseInstructions();
	if (m_parser.ParserErrors()) return false;

	return true;
}
