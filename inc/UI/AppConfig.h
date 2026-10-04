#pragma once

#include "UI/AppDefaults.h"

struct AppConfig
{
	bool executeAuto{ false };
	bool followPC{ true };
	bool showISA{ false };
	float autoSpeed{ 1.0f };

	void Reset()
	{
		executeAuto = AppDefaults::EXECUTE_AUTO;
		followPC = AppDefaults::FOLLOW_PC;
		showISA = AppDefaults::SHOW_ISA;
		autoSpeed = AppDefaults::AUTO_SPEED;
	}
};