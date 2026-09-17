#include "UI/Application.h"

Application::Application() :
	m_CPU{ std::make_unique<CPU>() },
	m_dissasembler{ std::make_unique<Dissasembler>() },
	m_editorMode{ EditorMode::DISSASEMBLY },
	m_executeAuto{ false },
	m_followPC{ true },
	m_showISA{ false },
	m_autoSpeed{ 1.0f }
{

}

const CPU& Application::GetCPU() const
{
	return *m_CPU;
}

void Application::Init()
{
}

void Application::Run()
{
	sf::Clock instructionCycle;

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
					m_CPU->Step();
				}
				else if (keyCode->code == sf::Keyboard::Key::R)
				{
					m_CPU->GetMemoryUnit().Clear();
					m_CPU->Reset();
					m_followPC = true;
					m_executeAuto = false;
				}
			}

			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		if (m_executeAuto && instructionCycle.getElapsedTime().asSeconds() >= m_autoSpeed)
		{
			m_CPU->Step();
			instructionCycle.restart();
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		window.clear(sf::Color(65, 65, 65));

		UIEditor::DrawCpuState(*m_CPU);
		UIEditor::DrawAssemblyPanel(m_editorMode, m_CPU->GetMemoryUnit(), *m_dissasembler, *m_CPU);
		UIEditor::DrawMemoryView(m_CPU->GetMemoryUnit(), *m_CPU, m_followPC);
		UIEditor::DrawMenu(m_CPU->GetMemoryUnit(), m_executeAuto, m_followPC, *m_CPU);
		UIEditor::DrawSpeedSlider(m_autoSpeed);
		UIEditor::DrawHelpMenu();

		ImGui::SFML::Render(window);
		window.display();
	}
	ImGui::SFML::Shutdown();
}