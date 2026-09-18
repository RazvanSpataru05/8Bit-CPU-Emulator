#include "Utils/CharacterUtils.h"

bool Utils::IsValidDigit(unsigned char c, uint8_t base)
{
    switch (base)
    {
    case 2:
        return c == '0' || c == '1';

    case 10:
        return std::isdigit(c);
    
    case 16:
        return std::isxdigit(c);

    default:
        return false;
    }
}
