#include "Assembler/Assembler.h"

Assembler::Assembler() :
	m_lexer{Lexer()},
	m_parser{Parser()}
{

}
