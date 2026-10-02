#include "UI/Terminal.h"

bool Terminal::ExecuteCommand(std::string_view line)
{
    const std::string command = Utils::RemoveWhiteSpace(Utils::ToLower(line));
    if (command == "clear")
    {
        Clear();
        return true;
    }

    return false;
}

void Terminal::Clear() noexcept
{
    m_lines.clear();
}

void Terminal::AddLine(const TerminalLine& line) noexcept
{
    m_lines.emplace_back(line);
}

std::span<const TerminalLine> Terminal::GetLines() const noexcept
{
    return m_lines;
}
