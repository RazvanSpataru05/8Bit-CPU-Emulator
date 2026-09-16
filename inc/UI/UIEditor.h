#pragma once

#include "Assembler/ISAEntry.h"
#include "Core/CPU.h"
#include "Core/MemoryUnit.h"
#include "Debugger/Dissasembler.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui-SFML.h"

enum class EditorMode
{
	EDIT = 0,
	DISSASEMBLY
};

namespace UIEditor
{
	void DrawCpuState(const CPU& cpu);
	void DrawAssemblyPanel(EditorMode& mode, const MemoryUnit& memoryUnit, 
		const Dissasembler& dissasembler, const CPU& cpu);
	void DrawMemoryView(const MemoryUnit& memoryUnit, CPU& cpu, bool& followPC);
	void DrawMenu(MemoryUnit& memoryUnit, 
		bool& executeAuto, bool& followPC, CPU& cpu);
	void DrawSpeedSlider(float& speed);
	void DrawHelpMenu();
}
