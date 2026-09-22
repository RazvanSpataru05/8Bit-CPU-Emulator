#include "Assembler/Parser.h"

using namespace ISA;

Parser::Parser(std::span <const Token> tokens) :
	m_lineNumber{ 1u },
	m_pos{ 0 }
{
	m_tokens.assign(tokens.begin(), tokens.end());
	ResetCurrentStatement();
}

bool Parser::ParserErrors() const noexcept
{
	return m_errors.empty();
}

std::span<const Statement> Parser::GetStatements() const noexcept
{
	return m_statements;
}

void Parser::SetTokens(std::span<const Token> tokens)
{
	m_tokens.assign(tokens.begin(), tokens.end());
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
		std::cout << "OPCODE: 0x" << std::hex << static_cast<int>(statement.opcode) << std::endl;
		std::cout << "VALUES: ";
		for (size_t index = 0; index < statement.operatorCount; ++index)
		{
			std::cout << "0x" << std::hex << static_cast<int>(statement.operands[index]) << " ";
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
	case OperatorKind::IMM_8:	{ m_currentStatement.operands = ConsumeImm8();		break; }
	case OperatorKind::ADDR_16: { m_currentStatement.operands = ConsumeAddr16();	break; }
	case OperatorKind::REG:		{ m_currentStatement.operands = ConsumeReg();		break; }
	case OperatorKind::REG_REG: { m_currentStatement.operands = ConsumeRegReg();	break; }
	default: break;
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

	operand[0] = static_cast<uint8_t>(ConsumeNumber());
	return operand;
}

std::array<uint8_t, 2> Parser::ConsumeAddr16()
{
	std::array<uint8_t, 2> operands{};

	const uint16_t address = static_cast<uint16_t>(ConsumeNumber());
	operands[0] = static_cast<uint8_t>(address >> 8); // hi part
	operands[1] = static_cast<uint8_t>(address & 0xFF); // lo part
	return operands;
}

std::array<uint8_t, 2> Parser::ConsumeReg()
{
	std::array<uint8_t, 2> operands{};
	operands[0] = ConsumeSelector();

	return operands;
}

std::array<uint8_t, 2> Parser::ConsumeRegReg()
{
	std::array<uint8_t, 2> operands{};
	operands[0] = ConsumeSelector();
	ExpectComma();
	operands[1] = ConsumeSelector();

	return operands;
}

void Parser::ExpectEndOfStatement()
{
	if (m_pos >= m_tokens.size()) return;
	const Token& token = Next();
	assert(token.type == TokenType::NEW_LINE || token.type == TokenType::END_OF_FILE);
}

void Parser::ExpectComma()
{
	const Token& token = Next();
	assert(token.type == TokenType::COMMA);
}

const Token& Parser::Next()
{
	return m_tokens[m_pos++];
}

uint32_t Parser::ConsumeNumber()
{
	const Token& token = Next();
	assert(token.type == TokenType::NUMBER);
	return Utils::ParseNumber(token.value);
}

uint8_t Parser::ConsumeSelector()
{
	const Token& token = Next();
	assert(token.type == TokenType::REGISTER || token.type == TokenType::NUMBER);
	if (token.type == TokenType::REGISTER)
	{
		const std::string upper = Utils::ToUpper(token.value);
		assert(nameToSelector.contains(upper));
		return nameToSelector.at(upper);
	}
	else // token type is number
	{
		const uint32_t selector = (Utils::ParseNumber(token.value));
		assert(selectorToName.contains(static_cast<uint8_t>(selector)));
		return static_cast<uint8_t>(selector);
	}
}
