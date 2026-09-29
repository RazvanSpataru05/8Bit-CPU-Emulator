#include "Assembler/Parser.h"

using namespace ISA;

Parser::Parser(std::span<const Token> tokens) :
	m_pos{ 0 }
{
	m_tokens.assign(tokens.begin(), tokens.end());
	ResetCurrentStatement();
}

bool Parser::ParserErrors() const noexcept
{
	return !m_errors.empty();
}

std::span<const Statement> Parser::GetStatements() const noexcept
{
	return m_statements;
}

std::span<const AssemblerError> Parser::GetErrors() const noexcept
{
	return m_errors;
}

void Parser::SetTokens(std::span<const Token> tokens)
{
	m_tokens.assign(tokens.begin(), tokens.end());
	std::cout << "Tokens: " << m_tokens.size() << std::endl;
}

void Parser::BuildSymbolTable()
{
	std::cout << "FIRST PASS\n\n";
	m_labels.clear();
	m_pos = 0;
	m_currentAddress = 0x0000;

	std::cout << "In" << std::endl;
	while (m_pos < m_tokens.size() && Peek().type != TokenType::END_OF_FILE)
	{
		std::cout << "Position: " << m_pos << " " << m_tokens[m_pos].value << std::endl;

		if (Peek().type == TokenType::NEW_LINE) { Next(); continue; }

		if (Peek().type == TokenType::IDENTIFIER)
		{
			HandleLabel();
			continue;
		}

		const ISAEntry* entry = ISA::Find(Utils::ToUpper(Peek().value));
		if (!entry) continue;

		m_currentAddress += entry->size;
		SkipOperandTokens(entry->operatorKind);
		ExpectEndOfStatement();
	}
	std::cout << "Out" << std::endl;
}

void Parser::ParseInstructions()
{
	BuildSymbolTable();
	PrintLabels();

	std::cout << "SECOND PASS\n\n";
	if (!m_errors.empty())
	{
		std::cout << "We have parser errors\n\n\n";
	}
	m_statements.clear();
	m_errors.clear();
	m_pos = 0;
	m_currentAddress = 0x0000;
	ResetCurrentStatement();

	while (m_pos < m_tokens.size() && Peek().type != TokenType::END_OF_FILE)
	{
		const Token& currentToken = Peek();
		std::cout << "Value: " << Peek().value << std::endl;
		std::cout << "Token type: " << Utils::TokenTypeToString(Peek()) << std::endl << std::endl;

		auto it = std::find_if(m_handlers.begin(), m_handlers.end(), [currentToken](const auto& handler) {
			return handler.first(currentToken);
			});
		if (it != m_handlers.end())
		{
			it->second(currentToken);
		}
		else
		{
			AddError("Parser Error at line " + std::to_string(currentToken.line) + ", column " + std::to_string(currentToken.column) +
				": '" + currentToken.value + "' is not a recognized instruction.");
			ConsumeLine();
		}
	}
}

void Parser::AddStatement()
{
	m_statements.emplace_back(m_currentStatement);
}

void Parser::AddError(std::string_view message)
{
	m_errors.emplace_back(Severity::ERROR, Stage::PARSER, Peek().line, Peek().column, message);
}

void Parser::ResetCurrentStatement()
{
	m_currentStatement.opcode = 0x00;
	m_currentStatement.operatorCount = 0u;
	m_currentStatement.operands.fill(0x00);
	m_currentStatement.ISAEntry = nullptr;
}

void Parser::PrintStatements() const noexcept
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

void Parser::PrintLabels() const noexcept
{
	std::cout << "Labels: " << m_labels.size() << std::endl;
	for (auto it = m_labels.begin(); it != m_labels.end(); it++)
	{
		std::cout << "Label: " << it->first << std::endl;
		std::cout << "Address: " << it->second.address << std::endl;
		std::cout << "Line declaration: " << it->second.lineDeclaration << std::endl;
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
	case OperatorKind::NONE: { Next(); break; }
	case OperatorKind::IMM_8: { m_currentStatement.operands = ConsumeImm8();		break; }
	case OperatorKind::ADDR_16: { m_currentStatement.operands = ConsumeAddr16();	break; }
	case OperatorKind::REG: { m_currentStatement.operands = ConsumeReg();			break; }
	case OperatorKind::REG_REG: { m_currentStatement.operands = ConsumeRegReg();	break; }
	default: break;
	}

	ExpectEndOfStatement();
	AddStatement();
}

void Parser::HandleIdentifierToken(const Token& token)
{
	ConsumeLabel();
}

void Parser::HandleNewLineToken(const Token& token)
{
	ResetCurrentStatement();
	Next();
}

void Parser::HandleLabel()
{
	if (IsLabelDefinition())
	{
		const std::string label = Utils::ToLower(Peek().value);
		if (m_labels.find(label) != m_labels.end())
		{
			AddError("Parser Error at line " + std::to_string(Peek().line) + ", column " + std::to_string(Peek().column)
				+ ": '" + std::string(Peek().value) + "' already exists.");
		}
		else
		{
			m_labels.insert({ label, LabelInfo(m_currentAddress, Peek().line) });
		}
		ConsumeLabel();
	}
	else
	{
		ConsumeLine();
	}
}

void Parser::SkipOperandTokens(OperatorKind operatorKind)
{
	switch (operatorKind)
	{
	case OperatorKind::NONE: { ++m_pos; return; } // mnemonic
	case OperatorKind::REG_REG: { m_pos += 4; return; } // mnemonic, first reg, comma, second reg 
	case OperatorKind::IMM_8:
	case OperatorKind::ADDR_16:
	case OperatorKind::REG:
	{
		m_pos += 2; return; // mnemonic, operand
	}
	default: return;
	}
}

std::array<uint8_t, 2> Parser::ConsumeImm8()
{
	std::array<uint8_t, 2> operand{};

	operand[0] = static_cast<uint8_t>(ConsumeNumber());
	Next();
	return operand;
}

std::array<uint8_t, 2> Parser::ConsumeAddr16()
{
	std::array<uint8_t, 2> operands{};
	uint16_t address{};
	const Token& token = Next();

	if (token.type == TokenType::IDENTIFIER)
	{
		const std::string label = Utils::ToLower(Peek().value);
		assert(m_labels.contains(label));
		address = m_labels.at(label).address;
	}
	else if (token.type == TokenType::NUMBER)
	{
		address = static_cast<uint16_t>(ConsumeNumber());
	}
	else
	{
		AddError("Parser Error at line " + std::to_string(token.line) + ", column " + std::to_string(token.column) +
			": expected 16-bit address or label, found " + Utils::TokenTypeToString(token) + ".");
	}

	operands[0] = static_cast<uint8_t>(address >> 8); // hi part
	operands[1] = static_cast<uint8_t>(address & 0xFF); // lo part

	Next();
	return operands;
}

std::array<uint8_t, 2> Parser::ConsumeReg()
{
	std::array<uint8_t, 2> operands{};
	operands[0] = ConsumeSelector();

	Next();
	return operands;
}

std::array<uint8_t, 2> Parser::ConsumeRegReg()
{
	std::array<uint8_t, 2> operands{};
	operands[0] = ConsumeSelector();
	ExpectComma();
	operands[1] = ConsumeSelector();

	Next();
	return operands;
}

void Parser::ExpectEndOfStatement()
{
	if (m_pos >= m_tokens.size()) return;

	if (Peek().type != TokenType::NEW_LINE && Peek().type != TokenType::END_OF_FILE)
	{
		AddError("Parser Error at line " + std::to_string(Peek().line) + ", column " + std::to_string(Peek().column) +
			": expected newline or end of file at the end of statement. Found " + Utils::TokenTypeToString(Peek()) + ".");
	}

	if (m_pos + 1 < m_tokens.size())
	{
		Next();
	}
}

void Parser::ExpectComma()
{
	if (Next().type != TokenType::COMMA)
	{
		AddError("Parser Error at line " + std::to_string(Peek().line) + ", column " + std::to_string(Peek().column) +
		": expected ',' after first operand of 'MOV', found " + Utils::TokenTypeToString(Peek()) + ".");
	}
}

void Parser::ExpectColon()
{
	if (Next().type != TokenType::COLON)
	{
		AddError("Parser Error at line " + std::to_string(Peek().line) + ", column " + std::to_string(Peek().line) +
			": expected ':' after label definition, found " + Utils::TokenTypeToString(Peek()) + ".");
	}
}

void Parser::ConsumeLabel()
{
	Next(); Next();
}

bool Parser::IsLabelDefinition() const
{
	return m_pos + 1 < m_tokens.size() && m_tokens[m_pos + 1].type == TokenType::COLON;
}

const Token& Parser::Peek() const
{
	return m_tokens[m_pos];
}

const Token& Parser::Next()
{
	return m_tokens[++m_pos];
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

	if (token.type == TokenType::REGISTER)
	{
		const std::string upper = Utils::ToUpper(token.value);
		if (!nameToSelector.contains(upper))
		{
			AddError("Parser Error at line " + std::to_string(token.line) + ", column " + std::to_string(token.column) +
				": '" + token.value + "' is not a valid register name (valid range: A-D).");
		}
		return nameToSelector.at(upper);
	}
	else if (token.type == TokenType::NUMBER)
	{
		const uint32_t selector = (Utils::ParseNumber(token.value));
		if (!selectorToName.contains(static_cast<uint8_t>(selector)))
		{
			AddError("Parser Error at line " + std::to_string(token.line) + ", column " + std::to_string(token.column) +
				": '" + std::to_string(static_cast<int>(selector)) + "' is not a valid register selector (valid range: 0x00-0x03).");
		}
		return static_cast<uint8_t>(selector);
	}
	else
	{
		AddError("Parser Error at line " + std::to_string(token.line) + ", column " + std::to_string(token.column) +
			": expected register selector or name, found " + Utils::TokenTypeToString(Peek()) + ".");
	}
}

void Parser::ConsumeLine()
{
	while (Peek().type != TokenType::NEW_LINE && Peek().type != TokenType::END_OF_FILE)
	{
		Next();
	}
	if (Peek().type == TokenType::NEW_LINE) { Next(); }
}
