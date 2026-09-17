#include "IO/ProgramLoader.h"

std::vector<uint8_t> ProgramLoader::ParseHexValues(const std::filesystem::path& path, bool& hasData)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Error: could not open file " + path.string());
    }
    
    std::vector<uint8_t> values;
    unsigned hexValue{};
    while (file >> std::hex >> hexValue)
    {
        values.emplace_back(static_cast<uint8_t>(hexValue));
        if (hexValue != 0x00)
        {
            hasData = true;
        }
    }
    file.close();
    return values;
}
