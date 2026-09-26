#pragma once

#include "Assembler/ISAEntry.h"

#include "Core/CPU.h"
#include "Core/MemoryUnit.h"

#include "Debugger/Dissasembler.h"

#include "Utils/StringUtils.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui-SFML.h"

class Assembler;

namespace UIEditor
{
	enum class Mode : uint8_t
	{
		EDIT = 0u,
		DISSASEMBLY
	};

	enum class PageType : uint8_t
	{
		GLOSSARY_PAGE = 1u,
		LOAD_STORE_PAGE,
		ARITHMETIC_PAGE,
		LOGICAL_PAGE,
		COMPARE_PAGE,
		JUMP_PAGE,
		STACK_PAGE,
		MISC_PAGE
	};

	void DrawCPUState(const CPU& cpu);
	void DrawAssemblyPanel(Mode& mode, const MemoryUnit& memoryUnit, 
						   const Dissasembler& dissasembler, CPU& cpu, Assembler& assembler);
	void DrawMemoryView(const MemoryUnit& memoryUnit, CPU& cpu, bool& followPC);
	void DrawMenu(MemoryUnit& memoryUnit, 
				  bool& executeAuto, bool& followPC, CPU& cpu);
	void DrawSpeedSlider(float& speed);
	void DrawHelpMenu();
	void DrawErrorList(const Assembler& assembler);
}
