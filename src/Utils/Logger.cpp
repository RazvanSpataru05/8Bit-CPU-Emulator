#include "Utils/Logger.h"

namespace Logger
{
	namespace
	{
		const std::string LOG_FILE_NAME{ "console.log" };
	}

	void AddInfoMessage(std::string_view message)
	{
		static std::ofstream out(LOG_FILE_NAME, std::ios::app);
		out << message;
		out.flush();
	}

	void ClearLogFile()
	{
		std::ofstream out(LOG_FILE_NAME, std::ios::trunc);
	}
}


