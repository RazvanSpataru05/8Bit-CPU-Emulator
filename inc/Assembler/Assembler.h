#pragma once

#include "Assembler/AssemblerError.h"
#include "Assembler/Parser.h"

class Assembler
{
public:
	Assembler();

	bool Assemble(std::string_view sourceCode);

	[[nodiscard]] std::span<const Statement>		GetStatements()			const noexcept;
	[[nodiscard]] std::span<const AssemblerError>	GetAssemblerErrors()	const noexcept;

private:
	Assembler(const Assembler&) = delete;
	Assembler& operator=(const Assembler&) = delete;
	Assembler(Assembler&&) = delete;
	Assembler& operator=(Assembler&&) = delete;

private:
	Lexer m_lexer;
	Parser m_parser;

	std::vector<AssemblerError> m_errors;
};