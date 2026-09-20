#include "Assembler/Parser.h"

Parser::Parser(const std::vector<Token>& tokens) :
	m_tokens{ tokens },
	m_lineNumber{ 1u },
	m_currentTokenIndex{ 0 }
{
	ResetCurrentStatement();
}

void Parser::Parse()
{
	m_statements.clear();
	m_lineNumber = 1u;
	m_currentTokenIndex = 0;
	ResetCurrentStatement();

	while (m_currentTokenIndex < m_tokens.size())
	{
		const Token currentToken = m_tokens[m_currentTokenIndex];

	}
}

void Parser::BuildStatement()
{
	
}

void Parser::AddStatement()
{
	m_statements.emplace_back(m_currentStatement);
}

void Parser::ResetCurrentStatement()
{
	m_currentStatement.opcode = 0x00;
	m_currentStatement.operands.fill(0x00);
	m_currentStatement.ISAEntry = nullptr;
}

void Parser::PrintStatements() const
{
	for (const auto& statement : m_statements)
	{
		std::cout << "MNEMONIC: " << statement.ISAEntry->mnemonic << std::endl;
		std::cout << "OPCODE: " << statement.opcode << std::endl;
		std::cout << "VALUES: ";
		for (uint8_t value : statement.operands)
		{
			std::cout << value << " ";
		}
		std::cout << std::endl << std::endl;
	}
}

void Parser::HandleMnemonicToken(const Token& token)
{
}

void Parser::HandleIdentifierToken(const Token& token)
{

}

void Parser::HandleNewLineToken(const Token& token)
{
	ResetCurrentStatement();

}
