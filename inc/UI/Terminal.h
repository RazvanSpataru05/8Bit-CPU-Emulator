#pragma once

#include <vector>
#include <string>
#include <span>

class Terminal
{
public:
	Terminal() = default;

	void Clear() noexcept;
	void AddLine(const std::string& line) noexcept;

	const std::vector<std::string>& GetLines() const noexcept;

private:
	Terminal(const Terminal&) = delete;
	Terminal& operator=(const Terminal&) = delete;

	Terminal(Terminal&&) = delete;
	Terminal& operator=(Terminal&&) = delete;

private:
	
	std::vector<std::string> m_lines;
};