#pragma once

#include <fstream>
#include <filesystem>

namespace Logger
{
	void AddInfoMessage(std::string_view message);
	void ClearLogFile();
}