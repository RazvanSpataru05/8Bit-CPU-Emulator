#include "UI/UIEditor.h"
#include "Assembler/Lexer.h"

uint8_t helperPage = 1u;
uint16_t currentPage{};

constexpr uint16_t PAGE_SIZE = 256u;
bool visibility = false;

constexpr uint8_t FIRST_LOAD_INSTRUCTION = 0x01;
constexpr uint8_t LAST_LOAD_INSTRUCTION = 0x14;

constexpr uint8_t FIRST_ARITHMETIC_INSTRUCTION = 0x20;
constexpr uint8_t LAST_ARITHMETIC_INSTRUCTION = 0x2B;

constexpr uint8_t FIRST_LOGIC_INSTRUCTION = 0x30;
constexpr uint8_t LAST_LOGIC_INSTRUCTION = 0x38;

constexpr uint8_t FIRST_COMPARE_INSTRUCTION = 0x40;
constexpr uint8_t LAST_COMPARE_INSTRUCTION = 0x41;

constexpr uint8_t FIRST_JUMP_INSTRUCTION = 0x50;
constexpr uint8_t LAST_JUMP_INSTRUCTION = 0x58;

constexpr uint8_t FIRST_STACK_INSTRUCTION = 0x60;
constexpr uint8_t LAST_STACK_INSTRUCTION = 0x63;

const size_t BUFFER_SIZE{ 8192 };

enum PageNumber
{
	NONE = 0,
	GLOSSARY_PAGE,
	LOAD_STORE_PAGE,
	ARITHMETIC_PAGE,
	LOGICAL_PAGE,
	COMPARE_PAGE,
	JUMP_PAGE,
	STACK_PAGE,
	MISC_PAGE
};


static const char* FlagToStr(bool value)
{
	return value ? "True" : "False";
}

static void SetupInstructionsTableColumn()
{
	ImGui::TableSetupColumn("Mnemonic", ImGuiTableColumnFlags_WidthFixed, 92.0f);
	ImGui::TableSetupColumn("Opcode", ImGuiTableColumnFlags_WidthFixed, 56.0f);
	ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 36.0f);
	ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthStretch);

	ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, ImVec4(0.18f, 0.28f, 0.45f, 1.0f));
	ImGui::TableHeadersRow();
	ImGui::PopStyleColor();
}

static void TextCentered(std::string_view text)
{
	float cellWidth = ImGui::GetColumnWidth();
	float textWidth = ImGui::CalcTextSize(text.data()).x;
	float offset = (cellWidth - textWidth) * 0.5f;

	if (offset > 0.0f)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);
	}
	ImGui::TextUnformatted(text.data(), text.data() + text.size());
}

static void DisplayInstruction(const ISAEntry* table, size_t index)
{
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.40f, 0.80f, 1.00f, 1.0f));
	TextCentered(table[index].mnemonic);
	ImGui::PopStyleColor();

	ImGui::TableSetColumnIndex(1);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00f, 0.75f, 0.20f, 1.0f));
	TextCentered(std::format("0x{:02X}", table[index].opcode));
	ImGui::PopStyleColor();

	ImGui::TableSetColumnIndex(2);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.55f, 0.95f, 0.55f, 1.0f));
	TextCentered(std::format("{}", table[index].size));
	ImGui::PopStyleColor();

	ImGui::TableSetColumnIndex(3);
	ImGui::TextWrapped("%s", table[index].description);
}

static void DisplayPageInstructions(uint8_t firstInstruction, uint8_t lastInstruction)
{
	SetupInstructionsTableColumn();

	int tableSize;
	const ISAEntry* table = GetISATable(tableSize);
	for (size_t index = 0; index < tableSize; ++index)
	{
		if (table[index].opcode >= firstInstruction && table[index].opcode <= lastInstruction)
			DisplayInstruction(table, index);
	}
}

static void DisplayPageInstructions(std::initializer_list<uint8_t> opcodes)
{
	SetupInstructionsTableColumn();

	int tableSize;
	const ISAEntry* table = GetISATable(tableSize);
	for (uint16_t index = 0; index < tableSize; ++index)
	{
		for (uint8_t opcode : opcodes)
		{
			if (table[index].opcode == opcode)
			{
				DisplayInstruction(table, index);
			}
		}
	}
}

static void DisplayTable(const char* pageTitle, const char* tableTitle,
	uint8_t firstInstruction, uint8_t lastInstruction)
{
	float windowWidth = ImGui::GetWindowSize().x;
	float titleWidth = ImGui::CalcTextSize(pageTitle).x;

	ImGui::SetCursorPosX((windowWidth - titleWidth) * 0.5f);
	ImGui::SetCursorPosY(windowWidth * 0.05f);
	ImGui::TextColored(ImVec4(0.90f, 0.90f, 0.90f, 1.0f), pageTitle);
	ImGui::Spacing();
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.35f, 0.55f, 0.85f, 1.0f));
	ImGui::Separator();
	ImGui::PopStyleColor();
	ImGui::Spacing();

	ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(8.0f, 5.0f));
	if (ImGui::BeginTable(tableTitle, 4, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg |
		ImGuiTableFlags_PadOuterX))
	{
		DisplayPageInstructions(firstInstruction, lastInstruction);
		ImGui::EndTable();
	}
	ImGui::PopStyleVar();
}

static void DisplayTable(const char* pageTitle, const char* tableTitle, std::initializer_list<uint8_t> opcodes)
{
	float titleWidth = ImGui::CalcTextSize(pageTitle).x;
	float windowWidth = ImGui::GetWindowSize().x;

	ImGui::SetCursorPosX((windowWidth - titleWidth) * 0.5f);
	ImGui::SetCursorPosY(windowWidth * 0.05f);
	ImGui::TextColored(ImVec4(0.90f, 0.90f, 0.90f, 1.0f), pageTitle);
	ImGui::Spacing();
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.35f, 0.55f, 0.85f, 1.0f));
	ImGui::Separator();
	ImGui::PopStyleColor();
	ImGui::Spacing();

	ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(8.0f, 5.0f));
	if (ImGui::BeginTable(tableTitle, 4, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg |
		ImGuiTableFlags_PadOuterX))
	{
		DisplayPageInstructions(opcodes);
		ImGui::EndTable();
	}
	ImGui::PopStyleVar();
}

namespace UIEditor
{
	void DrawCpuState(const CPU& cpu)
	{
		ImGui::Begin("CPU State");
		ImGui::Text("Program Counter: 0x%04X", cpu.GetPC());
		ImGui::Text("Stack Pointer: 0x%02X", cpu.GetSP());
		ImGui::Text("Instruction Register: 0x%02X", cpu.GetIR());
		ImGui::Separator();

		ImGui::Text("\nRegistry");
		ImGui::Text("A: %d", cpu.GetA());
		ImGui::Text("B: %d", cpu.GetB());
		ImGui::Text("C: %d", cpu.GetC());
		ImGui::Text("D: %d", cpu.GetD());
		ImGui::Separator();

		ImGui::Text("\nFlags");
		ImGui::Text("Zero Flag: %s", FlagToStr(cpu.GetZeroFlag()));
		ImGui::Text("Carry Flag: %s", FlagToStr(cpu.GetCarryFlag()));
		ImGui::Text("Negative Flag: %s", FlagToStr(cpu.GetNegativeFlag()));
		ImGui::Text("Overflow Flag: %s", FlagToStr(cpu.GetOverflowFlag()));
		ImGui::Text("Halt Flag: %s", FlagToStr(cpu.GetHaltFlag()));
		ImGui::Separator();
		ImGui::End();
	}

	void DrawAssemblyPanel(EditorMode& mode, const MemoryUnit& memoryUnit, const Dissasembler& dissasembler, const CPU& cpu)
	{
		static char editorBuffer[BUFFER_SIZE];

		ImGui::Begin("Assembly Panel");
		if (ImGui::Button("Edit Mode"))
		{
			mode = EditorMode::EDIT;
		}
		ImGui::SameLine();
		if (ImGui::Button("Dissasembly"))
		{
			mode = EditorMode::DISSASEMBLY;
		}
		ImGui::Separator();

		if (mode == EditorMode::EDIT)
		{
			ImGui::InputTextMultiline
			("##editor", editorBuffer, sizeof(editorBuffer), ImVec2(-1, 300), ImGuiInputTextFlags_AllowTabInput);
			if (ImGui::Button("Assemble & Load"))
			{
				Lexer lexer{ editorBuffer };
				lexer.Tokenize();
				const std::string result = lexer.GetTokenizedSourceCode();
				strncpy_s(editorBuffer, sizeof(editorBuffer), result.c_str(), _TRUNCATE);
				lexer.PrintTokenizedSourceCode();
			}
		}
		else if (mode == EditorMode::DISSASEMBLY)
		{
			if (!memoryUnit.IsMemoryEmpty())
			{
				for (size_t index = 0; index < cpu.MEMORY_UNIT_SIZE; ++index)
				{
					uint8_t opcode = memoryUnit.Read(static_cast<uint16_t>(index));
					const InstructionDef instruction = dissasembler.GetInstructionDef(opcode);

					if (index == cpu.GetPC())
						ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
					else
						ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));

					switch (instruction.size)
					{
					case 1:
					{
						ImGui::Text("0x%02x: %s", opcode, instruction.mnemonic.c_str());
						break;
					}
					case 2:
					{
						uint8_t secondByte = memoryUnit.Read(static_cast<uint16_t>(index + 1));
						ImGui::Text("0x%02x: %s %d", opcode, instruction.mnemonic.c_str(), secondByte);
						break;
					}
					case 3:
					{
						uint8_t secondByte = memoryUnit.Read(static_cast<uint16_t>(index + 1));
						uint8_t thirdByte = memoryUnit.Read(static_cast<uint16_t>(index + 2));
						uint16_t address = (secondByte << 8) | thirdByte;
						ImGui::Text("0x%02x: %s 0x%04X", opcode, instruction.mnemonic.c_str(), address);
						break;
					}
					default:
						ImGui::Text("0x%04zx: ???", index);
						break;
					}
					index += static_cast<size_t>(instruction.size - 1);
					ImGui::PopStyleColor();
					if (opcode == 0xFF) break;
				}
			}
		}
		ImGui::End();
	}

	void DrawMemoryView(const MemoryUnit& memoryUnit, CPU& cpu, bool& followPC)
	{
		if (followPC)
		{
			currentPage = cpu.GetPC() / PAGE_SIZE;
		}

		std::string memoryView = "Memory View (page " + std::to_string(currentPage) + "/" + std::to_string(PAGE_SIZE - 1) + ")";
		ImGui::Begin(memoryView.c_str());
		if (ImGui::Button("\t\t\tPrev\t\t\t"))
			currentPage = currentPage - 1 < 0 ? 255 : currentPage - 1;

		ImGui::SameLine();
		const std::string spaces = std::string(" ", 79);
		ImGui::Text(spaces.c_str());
		ImGui::SameLine();
		if (ImGui::Button("\t\t\tNext\t\t\t"))
			currentPage = (currentPage + 1) % 256;

		const size_t startAddress = static_cast<size_t>(currentPage * PAGE_SIZE);
		const size_t endAddress = startAddress + PAGE_SIZE;
		if (ImGui::BeginTable("MemoryTable", 16, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			for (size_t index = startAddress; index < endAddress; ++index)
			{
				ImGui::TableNextColumn();
				if (index == cpu.GetPC() && !cpu.GetHaltFlag())
				{
					ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(0, 0, 255, 255));
				}

				if (cpu.IsReadingInstruction() && index == cpu.ComputeAddress(cpu.GetPC() + 1))
				{
					ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(0, 255, 0, 255));
				}
				else if (cpu.IsWritingInstruction() &&
					index == cpu.ComputeAddress(cpu.GetPC() + 1))
				{
					ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(255, 0, 0, 255));
				}

				ImGui::Text("0x%02X", memoryUnit.Read(static_cast<uint16_t>(index)));
			}
			ImGui::EndTable();
		}
		ImGui::End();
	}
	void DrawMenu(MemoryUnit& memoryUnit,
		bool& executeAuto, bool& followPC, CPU& cpu)
	{
		std::string execute = "Auto (";
		execute += executeAuto ? "ON)" : "OFF)";
		std::string follow = "Follow PC (";
		follow += followPC ? "ON)" : "OFF)";

		ImGui::Begin("Menu");
		if (ImGui::Button("\t\tNext Step\t\t"))
		{
			cpu.Step();
		}
		ImGui::SameLine();

		if (ImGui::Button("\t\tReset\t\t"))
		{
			memoryUnit.Clear();
			cpu.Reset();
			executeAuto = false;
			followPC = true;
		}

		if (ImGui::Button(execute.c_str()))
		{
			executeAuto = !executeAuto;
		}
		ImGui::SameLine();

		if (ImGui::Button(follow.c_str()))
		{
			followPC = !followPC;
		}
		ImGui::End();

		ImGui::Begin("Help");
		if (ImGui::Button("Help"))
		{
			visibility = true;
		}
		ImGui::End();
	}
	void DrawSpeedSlider(float& speed)
	{
		ImGui::Begin("Speed Slider");
		ImGui::SliderFloat("Speed (seconds)", &speed, 0.1f, 2.0f);
		ImGui::End();
	}
	void DrawHelpMenu()
	{
		if (visibility)
		{
			ImGui::Begin("Help Menu");
			std::string pageText = "Page " + std::to_string(helperPage) + "/8";
			float windowWidth = ImGui::GetWindowSize().x;
			float textWidth = ImGui::CalcTextSize(pageText.c_str()).x;
			float navButtonWidth = ImGui::CalcTextSize(">").x + ImGui::GetStyle().FramePadding.x * 2;
			float buttonWidth = ImGui::CalcTextSize("X").x + ImGui::GetStyle().FramePadding.x * 2;

			float centerX = (windowWidth - textWidth) * 0.5f;
			ImGui::SetCursorPosX(centerX);
			ImGui::Text(pageText.c_str());
			ImGui::SameLine();

			ImGui::SetCursorPosX(windowWidth - buttonWidth - ImGui::GetStyle().WindowPadding.x * 2.0f);
			if (ImGui::Button("X"))
			{
				visibility = false;
			}

			ImGui::SetCursorPosY(windowWidth * 0.055f - ImGui::GetStyle().WindowPadding.y * 1.2f);
			if (ImGui::Button("<") && helperPage > 1u)
				helperPage--;

			ImGui::SameLine();
			ImGui::SetCursorPosX(windowWidth - navButtonWidth - ImGui::GetStyle().WindowPadding.x * 2.0f);
			ImGui::SetCursorPosY(windowWidth * 0.055f - ImGui::GetStyle().WindowPadding.y * 1.2f);
			if (ImGui::Button(">") && helperPage < 8)
				helperPage++;

			switch (helperPage)
			{
			case PageNumber::GLOSSARY_PAGE:
			{
				const char* title = "Glossary";
				float titleWidth = ImGui::CalcTextSize(title).x;
				ImGui::SetCursorPosX((windowWidth - titleWidth) * 0.5f);
				ImGui::SetCursorPosY(windowWidth * 0.05f);
				ImGui::Text(title);
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				if (ImGui::BeginTable("glossary_table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg))
				{
					ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 120.0f);
					ImGui::TableSetupColumn("Definition", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableHeadersRow();

					static const std::pair<const char*, const char*> glossary[] =
					{
						{"PC", "Program Counter"},
						{"IR", "Instruction Register"},
						{"SP", "Stack Pointer"},
						{"A", "Accumulator Register"},
						{"B", ""},
						{"C", "Counter Register"},
						{"D", ""}
					};

					for (const auto& [mnemonic, definition] : glossary)
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), mnemonic);
						ImGui::TableSetColumnIndex(1);
						ImGui::TextWrapped(definition);
					}
					ImGui::EndTable();
				}
				break;
			}
			case PageNumber::LOAD_STORE_PAGE:
			{
				DisplayTable("Load & Store Instructions Table", "load_store_table", FIRST_LOAD_INSTRUCTION, LAST_LOAD_INSTRUCTION);
				break;
			}
			case PageNumber::ARITHMETIC_PAGE:
			{
				DisplayTable("Arithmetic Instructions Table", "arithmetic_table", FIRST_ARITHMETIC_INSTRUCTION, LAST_ARITHMETIC_INSTRUCTION);
				break;
			}
			case PageNumber::LOGICAL_PAGE:
			{
				DisplayTable("Logical Instructions Table", "logical_tabe", FIRST_LOGIC_INSTRUCTION, LAST_LOGIC_INSTRUCTION);
				break;
			}
			case PageNumber::COMPARE_PAGE:
			{
				DisplayTable("Compare Instructions Table", "compare_table", FIRST_COMPARE_INSTRUCTION, LAST_COMPARE_INSTRUCTION);
				break;
			}
			case PageNumber::JUMP_PAGE:
			{
				DisplayTable("Jump Instructions Table", "jump_table", FIRST_JUMP_INSTRUCTION, LAST_JUMP_INSTRUCTION);
				break;
			}
			case PageNumber::STACK_PAGE:
			{
				DisplayTable("Stack Instructions Table", "stack_table", FIRST_STACK_INSTRUCTION, LAST_STACK_INSTRUCTION);
				break;
			}
			case PageNumber::MISC_PAGE:
			{
				DisplayTable("Misc Instructions Table", "misc_table", { 0x00, 0x70, 0xFF });
				break;
			}
			}
			ImGui::End();
		}
	}
}