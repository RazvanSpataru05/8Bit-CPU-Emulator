#include "Utils/Logger.h"

void Logger::AddInfoMessage(std::string_view message)
{
	std::ofstream out("console.log", std::ios::app);
	out << message;
}
