#include "Core/Terminal.h"

void Terminal::ExecuteCommand(std::string_view line)
{
    const std::string command = Utils::RemoveWhiteSpace(Utils::ToLower(line));
    AddLine({ std::format("> {}", line), LineType::INFO });

    if (command == "clear")
    {
        Clear();
        return;
    }

    if (command == "error")
    {
        AddLine({std::format("{} : The term '{}' is not recognized as the name of a cmdlet, function, script file, or operable program.", 
            line, line), LineType::ERROR});
        return;
    }
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
