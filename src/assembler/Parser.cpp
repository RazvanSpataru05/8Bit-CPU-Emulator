#include "Assembler/Parser.h"

using namespace ISA;

Parser::Parser(const std::vector<Token>& tokens) :
	m_tokens{ tokens },
	m_lineNumber{ 1u },
	m_pos{ 0 }
{
	ResetCurrentStatement();
}

void Parser::ParseInstructions()
{
	m_statements.clear();
	m_lineNumber = 1u;
	m_pos = 0;
	ResetCurrentStatement();

	while (m_pos < m_tokens.size())
	{
		const Token& currentToken = Next();
		auto it = std::find_if(m_handlers.begin(), m_handlers.end(), [currentToken](const auto& handler) {
			return handler.first(currentToken);
			});
		if (it != m_handlers.end())
		{
			it->second(currentToken);
		}
		else
		{
			// error handling
		}
	}
}

void Parser::AddStatement()
{
	m_statements.emplace_back(m_currentStatement);
}

void Parser::ResetCurrentStatement()
{
	m_currentStatement.opcode = 0x00;
	m_currentStatement.operatorCount = 0u;
	m_currentStatement.operands.fill(0x00);
	m_currentStatement.ISAEntry = nullptr;
}

void Parser::PrintStatements() const
{
	for (const auto& statement : m_statements)
	{
		if (statement.ISAEntry)
		{
			std::cout << "MNEMONIC: " << statement.ISAEntry->mnemonic << std::endl;
		}
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
	const ISA::ISAEntry* entry = ISA::Find(Utils::ToUpper(token.value));
	if (!entry) return;

	m_currentStatement.opcode = entry->opcode;
	m_currentStatement.ISAEntry = entry;
	m_currentStatement.operatorCount = entry->size - 1;

	switch (entry->operatorKind)
	{
	case OperatorKind::NONE: break;
	case OperatorKind::IMM_8: { m_currentStatement.operands = ConsumeImm8(); break; }
	case OperatorKind::ADDR_16: { m_currentStatement.operands = ConsumeAddr16(); break; }
	case OperatorKind::REG: {}
	}

	ExpectEndOfStatement();
	AddStatement();
}

void Parser::HandleIdentifierToken(const Token& token)
{

}

void Parser::HandleNewLineToken(const Token& token)
{
	ResetCurrentStatement();
	++m_lineNumber;
}

std::array<uint8_t, 2> Parser::ConsumeImm8()
{
	std::array<uint8_t, 2> operand{};
	const std::string& value = ConsumeNumber();

	operand[0] = static_cast<uint8_t>(Utils::ParseNumber(value));
	return operand;
}

std::array<uint8_t, 2> Parser::ConsumeAddr16()
{
	std::array<uint8_t, 2> operands{};
	const std::string& value = ConsumeNumber();

	const uint16_t address = Utils::ParseNumber(value);
	operands[0] = static_cast<uint8_t>(address >> 8); // hi part
	operands[1] = static_cast<uint8_t>(address & 0xFF); // lo part
	return operands;
}

std::array<uint8_t, 2> Parser::ConsumeReg()
{
	std::array<uint8_t, 2> operands{};
	auto value = ConsumeRegisterOrSelector();
	if (value.second == TokenType::REGISTER) 
	{
		assert(nameToSelector.contains(value.first));
		operands[0] = nameToSelector.at(value.first);
	}
	else if (value.second == TokenType::NUMBER) // number is registry identifier
	{
		uint8_t selector = Utils::ParseNumber(value.first);
		assert(selectorToName.contains(selector));
		operands[0] = selector;
	}

	return operands;
}

void Parser::ExpectEndOfStatement()
{
	if (m_pos >= m_tokens.size()) return;
	assert(m_tokens[m_pos].type == TokenType::NEW_LINE);
	++m_pos;
}

const Token& Parser::Next()
{
	return m_tokens[m_pos++];
}

const std::string& Parser::ConsumeNumber()
{
	const Token& token = Next();
	assert(token.type == TokenType::NUMBER);
	return token.value;
}

std::pair<const std::string&, TokenType> Parser::ConsumeRegisterOrSelector()
{
	const Token& token = Next();
	assert(token.type == TokenType::REGISTER || token.type == TokenType::NUMBER);
	return { token.value, token.type };
}
