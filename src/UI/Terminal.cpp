#include "UI/Terminal.h"

void Terminal::Clear() noexcept
{
    m_lines.clear();
}

void Terminal::AddLine(std::string_view message) noexcept
{
    m_lines.emplace_back(message);
}

std::span<const std::string> Terminal::GetLines() const noexcept
{
    return m_lines;
}
