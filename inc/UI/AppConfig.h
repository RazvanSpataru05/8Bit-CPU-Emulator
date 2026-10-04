#pragma once

struct AppConfig
{
	bool executeAuto{ false };
	bool followPC{ true };
	bool showISA{ false };
	float autoSpeed{ 1.0f };

	void Reset()
	{
		executeAuto = false;
		followPC = true;
		showISA = false;
		autoSpeed = 1.0f;
	}
};