#include "Assembler/Parser.h"

using namespace ISA;

Parser::Parser(std::span<const Token> tokens) :
	m_pos{ 0 },
	m_currentAddress{ false }
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

std::span<const AssemblerWarning> Parser::GetWarnings() const noexcept
{
	return m_warnings;
}

void Parser::SetTokens(std::span<const Token> tokens)
{
	m_tokens.assign(tokens.begin(), tokens.end());
}

void Parser::AddLabels()
{
	m_labels.clear();
	m_pos = 0;
	m_currentAddress = 0x0000;
	m_currentStatementHasError = false;

	Logger::AddInfoMessage("In First Pass\n");
	while (m_pos < m_tokens.size() && Peek().type != TokenType::END_OF_FILE)
	{
		Logger::AddInfoMessage(std::format("Position: {}, {}\n", m_pos, m_tokens[m_pos].value));

		if (Peek().type == TokenType::NEW_LINE) { Next(); continue; }

		if (Peek().type == TokenType::IDENTIFIER)
		{
			HandleLabel();
			continue;
		}

		const ISAEntry* entry = ISA::Find(Utils::ToUpper(Peek().value));
		if (!entry) continue;

		m_currentAddress += entry->size;
		ConsumeLine();
	}
	Logger::AddInfoMessage("Out First Pass\n");
}

void Parser::ParseTokens()
{
	AddLabels();
	PrintLabels();

	m_statements.clear();
	m_errors.clear();
	m_warnings.clear();

	m_pos = 0;
	m_currentAddress = 0x0000;
	m_currentStatementHasError = false;
	m_seenHLT = false;
	ResetCurrentStatement();

	while (m_pos < m_tokens.size() && Peek().type != TokenType::END_OF_FILE)
	{
		const Token& currentToken = Peek();

		Logger::AddInfoMessage(std::format("Value: {}\n", Peek().value));
		Logger::AddInfoMessage(std::format("Token type: {}\n", Utils::TokenTypeToString(Peek())));

		auto it = std::find_if(m_handlers.begin(), m_handlers.end(), [currentToken](const auto& handler) {
			return handler.first(currentToken);
			});
		if (it != m_handlers.end())
		{
			it->second(currentToken);
		}
		else
		{	
			AddError(std::format("Error at line {}, column {}: '{}' is not a recognized instruction",
				currentToken.line, currentToken.column, currentToken.value));
			ConsumeLine();
		}
	}
	CheckUnusedLabels();
	if (!m_seenHLT)
	{
		AddWarning(std::format("Warning: no 'HLT' instruction found anywhere in the program."));
	}
}

void Parser::AddStatement()
{
	m_statements.emplace_back(m_currentStatement);
}

void Parser::AddError(std::string_view message)
{
	m_errors.emplace_back(AssemblerStage::PARSER, Peek().line, Peek().column, message);
}

void Parser::AddWarning(std::string_view message)
{
	m_warnings.emplace_back(AssemblerStage::PARSER, Peek().line, Peek().column, message);
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
			Logger::AddInfoMessage(std::format("MNEMONIC: {}\n", statement.ISAEntry->mnemonic));
		}
		Logger::AddInfoMessage(std::format("OPCODE: 0x{}\n", static_cast<int>(statement.opcode)));
		Logger::AddInfoMessage("VALUE(S): ");
		for (size_t index = 0; index < statement.operatorCount; ++index)
		{
			Logger::AddInfoMessage(std::format("0x{}, ", static_cast<int>(statement.operands[index])));
		}
		Logger::AddInfoMessage("\n\n");
	}
}

void Parser::PrintLabels() const noexcept
{
	Logger::AddInfoMessage(std::format("Labels: {}\n\n", m_labels.size()));
	for (auto it = m_labels.begin(); it != m_labels.end(); it++)
	{
		Logger::AddInfoMessage(std::format("Label: {}\n", it->first));
		Logger::AddInfoMessage(std::format("Address: {}\n", static_cast<int>(it->second.address))); 
		Logger::AddInfoMessage(std::format("Line declaration: {}\n", static_cast<int>(it->second.lineDeclaration))); 
	}
}

void Parser::HandleMnemonicToken(const Token& token)
{
	const ISA::ISAEntry* entry = ISA::Find(Utils::ToUpper(token.value));
	if (!entry) return;

	m_currentStatement.opcode = entry->opcode;
	m_currentStatement.ISAEntry = entry;
	m_currentStatement.operatorCount = entry->size - 1;

	if (entry->mnemonic == "HLT") m_seenHLT = true;

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
			AddError(std::format("Error at line {}, column {}: '{}' already exists.",
				Peek().line, Peek().column, Peek().value));
			ConsumeLine();
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

std::array<uint8_t, 2> Parser::ConsumeImm8()
{
	std::array<uint8_t, 2> operand{};
	Next(); // go to imm8 value

	ParsedNumber parsedNumber = ConsumeNumber();
	if (!CheckNumericLimit<uint8_t>(parsedNumber)) return {};

	operand[0] = static_cast<uint8_t>(parsedNumber.value);
	Next(); // go to newline

	return operand;
}

std::array<uint8_t, 2> Parser::ConsumeAddr16()
{
	std::array<uint8_t, 2> operands{};
	uint16_t address{};
	const Token& token = Next(); // go to addr

	if (token.type == TokenType::IDENTIFIER)
	{
		const std::string label = Utils::ToLower(Peek().value);
		if (!m_labels.contains(label))
		{
			AddError(std::format("Error at line {}, {}: undefined label '{}'.",
				Peek().line, Peek().column, Peek().value));
			ConsumeLine();

			return {};
		}
		else
		{
			address = m_labels.at(label).address;
			m_labels.at(label).used = true;
			Next(); // go to newline
		}
		
	}
	else if (token.type == TokenType::NUMBER)
	{
		ParsedNumber parsedNumber = ConsumeNumber();
		if (!CheckNumericLimit<uint16_t>(parsedNumber)) return {};

		address = static_cast<uint16_t>(parsedNumber.value);
		Next();
	}
	else
	{
		AddError(std::format("Error at line {}, column {}: expected 16-bit address or label, found {}.",
			token.line, token.column, Utils::TokenTypeToString(token)));
		ConsumeLine();

		return {};
	}

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
	if (m_currentStatementHasError) return operands;

	ExpectComma();
	if (m_currentStatementHasError) return operands; 

	operands[1] = ConsumeSelector();

	return operands;
}

void Parser::ExpectEndOfStatement()
{
	if (Peek().type != TokenType::NEW_LINE && Peek().type != TokenType::END_OF_FILE)
	{

		AddError(std::format("Error at line {}, column {}: expected newline or end of file at the end of statement. Found {}.",
			Peek().line, Peek().column, Utils::TokenTypeToString(Peek())));
		ConsumeLine();
	}

	if (m_pos + 1 < m_tokens.size())
	{
		Next();
	}
}

void Parser::ExpectComma()
{
	if (Peek().type != TokenType::COMMA)
	{
		m_currentStatementHasError = true;

		AddError(std::format("Error at line {}, column {}: expected ',' after first operand of 'MOV', found {}.",
			Peek().line, Peek().column, Utils::TokenTypeToString(Peek())));
		ConsumeLine();
	}
}

void Parser::ExpectColon()
{
	if (Next().type != TokenType::COLON)
	{
		m_currentStatementHasError = true;

		AddError(std::format("Error at line {}, column {}: expected ':' after label definition, found {}.",
			Peek().line, Peek().column, Utils::TokenTypeToString(Peek())));
		ConsumeLine();
	}
}

void Parser::CheckUnusedLabels()
{
	for (const auto& [label, info] : m_labels)
	{
		if (!info.used)
		{
			AddWarning(std::format("Warning at line {}: label '{}' is defined but never used.",
				info.lineDeclaration, label));
		}
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

ParsedNumber Parser::ConsumeNumber()
{
	if (Peek().type != TokenType::NUMBER)
	{
		AddError(std::format("Error at line {}, column {}: expected number, found {}.",
			Peek().line, Peek().column, Utils::TokenTypeToString(Peek())));
		ConsumeLine();

		return { 0u, false };
	}

	return Utils::ParseNumber(Peek().value);
}

uint8_t Parser::ConsumeSelector()
{
	const Token& token = Next();

	if (token.type == TokenType::REGISTER)
	{
		const std::string upper = Utils::ToUpper(token.value);
		if (!nameToSelector.contains(upper))
		{
			m_currentStatementHasError = true;

			AddError(std::format("Error at line {}, column {}: '{}' is not a valid register name (valid range: A-D).",
				token.line, token.column, token.value));
			ConsumeLine();
		}

		Next();
		return nameToSelector.at(upper);
	}
	else if (token.type == TokenType::NUMBER)
	{
		ParsedNumber parsedNumber = ConsumeNumber();
		if (!CheckNumericLimit<uint8_t>(parsedNumber)) return 0u;

		if (!selectorToName.contains(static_cast<uint8_t>(parsedNumber.value)))
		{
			m_currentStatementHasError = true;

			AddError(std::format("Error at line {}, column {}: '{}' is not a valid register selector (valid range: 0x00-0x03).",
				token.line, token.column, parsedNumber.value));
			ConsumeLine();

			return 0u;
		}

		Next();
		return static_cast<uint8_t>(parsedNumber.value);
	}
	else
	{
		m_currentStatementHasError = true;

		AddError(std::format("Error at line {}, column {}: expected register selector or name, found {}.",
			token.line, token.column, Utils::TokenTypeToString(Peek())));
		ConsumeLine();

		return 0u;
	}
}

void Parser::ConsumeLine()
{
	Logger::AddInfoMessage("CONSUME LINE called\n\n");
	while (Peek().type != TokenType::NEW_LINE && Peek().type != TokenType::END_OF_FILE)
	{
		std::cout << m_pos << " ";
		Next();
	}
	Logger::AddInfoMessage("CONSUME LINE finished\n");
}
