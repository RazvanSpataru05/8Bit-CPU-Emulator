#include "Utils/StringUtils.h"

std::string Utils::ToUpper(std::string_view word)
{
    std::string upper{ word };
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    return upper;
}