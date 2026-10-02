#pragma once

#include "Core/TerminalLine.h"

#include "Utils/StringUtils.h"

#include <vector>
#include <span>

class Terminal
{
public:
	Terminal() = default;

	void ExecuteCommand(std::string_view line);

	void Clear() noexcept;
	void AddLine(const TerminalLine& line) noexcept;

	std::span<const TerminalLine> GetLines() const noexcept;

private:
	Terminal(const Terminal&) = delete;
	Terminal& operator=(const Terminal&) = delete;

	Terminal(Terminal&&) = delete;
	Terminal& operator=(Terminal&&) = delete;

private:
	
	std::vector<TerminalLine> m_lines;
};