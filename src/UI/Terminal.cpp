#include "UI/Terminal.h"

void Terminal::Clear() noexcept
{
    m_lines.clear();
}

void Terminal::AddLine(const std::string& line) noexcept
{
    m_lines.emplace_back(line);
}

const std::vector<std::string>& Terminal::GetLines() const noexcept
{
    return m_lines;
}
