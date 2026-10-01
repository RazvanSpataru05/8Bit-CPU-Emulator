#include "Utils/UIUtils.h"

namespace Utils
{
	bool ErrorListTab(SelectedTab tab)
	{
		return tab == SelectedTab::ERROR_LIST;
	}

	bool ConsoleListTab(SelectedTab tab)
	{
		return tab == SelectedTab::CONSOLE;
	}
}
