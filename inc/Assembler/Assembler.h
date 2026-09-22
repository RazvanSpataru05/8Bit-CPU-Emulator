#pragma once

#include "Assembler/Parser.h"

class Assembler
{
public:
	Assembler();

private:
	Assembler(const Assembler&) = delete;
	Assembler& operator=(const Assembler&) = delete;
	Assembler(Assembler&&) = delete;
	Assembler& operator=(Assembler&&) = delete;

private:
	Lexer m_lexer;
	Parser m_parser;
};