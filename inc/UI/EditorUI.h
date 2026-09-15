#pragma once

#include "CPU.h"
#include "MemoryUnit.h"
#include "Dissasembler.h"
#include "ISAEntry.h"

#include "imgui.h"
#include "imgui-SFML.h"

enum EditorMode
{
	EDIT = 0,
	DISSASEMBLY
};

namespace EditorUI
{
	void DrawCpuState(const CPU& cpu);
	void DrawAssemblyPanel(EditorMode& mode, const MemoryUnit& memoryUnit, const Dissasembler& dissasembler, const CPU& cpu);
	void DrawMemoryView(const MemoryUnit& memoryUnit, CPU& cpu, bool& followPC);
	void DrawMenu(MemoryUnit& memoryUnit, const std::array<uint8_t, 65536>& initialMemory, bool& executeAuto, bool& followPC, CPU& cpu);
	void DrawSpeedSlider(float& speed);
	void DrawHelpMenu();
}
