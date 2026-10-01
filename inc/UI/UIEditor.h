#pragma once

#include "Assembler/ISAEntry.h"

#include "Core/CPU.h"
#include "Core/MemoryUnit.h"

#include "Debugger/Dissasembler.h"

#include "UI/UICommon.h"

#include "Utils/StringUtils.h"
#include "Utils/Logger.h"
#include "Utils/UIUtils.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui-SFML.h"

class Assembler;
struct AssemblerError;
struct AssemblerWarning;

namespace UIEditor
{
	void DrawCPUState(const CPU& cpu);
	void DrawAssemblyPanel(Mode& mode, const MemoryUnit& memoryUnit, 
						   const Dissasembler& dissasembler, CPU& cpu, Assembler& assembler);
	void DrawMemoryView(const MemoryUnit& memoryUnit, CPU& cpu, bool& followPC);
	void DrawMenu(MemoryUnit& memoryUnit, 
				  bool& executeAuto, bool& followPC, CPU& cpu);
	void DrawSpeedSlider(float& speed);
	void DrawHelpMenu();
	void DrawOutput(const Assembler& assembler);

	void DrawErrorList(std::span<const AssemblerError> errors, std::span<const AssemblerWarning> warnings);
	void DrawConsole();
}
