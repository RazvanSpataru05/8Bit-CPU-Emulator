#include "Core/MemoryUnit.h"
#include "Debugger/Dissasembler.h"
#include "UI/UIEditor.h"

#include <SFML/Graphics.hpp>

#include <fcntl.h>
#include <sstream>

EditorMode mode = EditorMode::DISSASEMBLY;

bool executeAuto = false;
bool followPC = true;
bool showISA = false;
float speed = 1.0f;

int main()
{
	sf::Clock instructionCycle;
	MemoryUnit memory;
	memory.ParseValues("program.txt");
	const std::array<uint8_t, 65536> initialMemory = memory.GetMemory();

	CPU cpu(memory);
	Dissasembler dissasembler;

	sf::Font font;
	sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "CPU Emulator");
	window.setFramerateLimit(60);

	ImGui::SFML::Init(window);
	sf::Clock deltaClock;

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			ImGui::SFML::ProcessEvent(window, event.value());
			if (event->is<sf::Event::KeyPressed>())
			{
				const auto keyCode = event->getIf<sf::Event::KeyPressed>();
				if (keyCode->code == sf::Keyboard::Key::Space)
				{
					cpu.Step();
				}
				else if (keyCode->code == sf::Keyboard::Key::R)
				{
					memory.Clear(initialMemory);
					cpu.Reset();
					followPC = true;
					executeAuto = false;
				}
			}

			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		if (executeAuto && instructionCycle.getElapsedTime().asSeconds() >= speed)
		{
			cpu.Step();
			instructionCycle.restart();
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		window.clear(sf::Color(65, 65, 65));

		UIEditor::DrawCpuState(cpu);
		UIEditor::DrawAssemblyPanel(mode, memory, dissasembler, cpu);
		UIEditor::DrawMemoryView(memory, cpu, followPC);
		UIEditor::DrawMenu(memory, initialMemory, executeAuto, followPC, cpu);
		UIEditor::DrawSpeedSlider(speed);
		UIEditor::DrawHelpMenu();

		ImGui::SFML::Render(window);
		window.display();
	}
	ImGui::SFML::Shutdown();
}