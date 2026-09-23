#include "UI/Application.h"

Application::Application() :
	m_CPU{ std::make_unique<CPU>() },
	m_dissasembler{ std::make_unique<Dissasembler>() },
	m_assembler{std::make_unique<Assembler>()},
	m_editorMode{ UIEditor::Mode::EDIT },
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

const Assembler& Application::GetAssembler() const
{
	return *m_assembler;
}

void Application::Init()
{
	const sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	m_window = sf::RenderWindow(sf::VideoMode(desktop), "8-Bit CPU Emulator", sf::Style::Default);
	m_window.setFramerateLimit(60);
	ImGui::SFML::Init(m_window);
}

void Application::ProcessEvents()
{
	while (const std::optional event = m_window.pollEvent())
	{
		ImGui::SFML::ProcessEvent(m_window, event.value());
		if (event->is<sf::Event::KeyPressed>())
		{
			ProcessKeyStrokes(event);
		}

		if (event->is<sf::Event::Closed>())
		{
			m_window.close();
		}
	}
}

void Application::Update()
{
	if (m_executeAuto && m_instructionCycle.getElapsedTime().asSeconds() >= m_autoSpeed)
	{
		m_CPU->Step();
		m_instructionCycle.restart();
	}
	ImGui::SFML::Update(m_window, m_deltaClock.restart());
}

void Application::RenderUI()
{
	m_window.clear(sf::Color(65, 65, 65));

	UIEditor::DrawCpuState(*m_CPU);
	UIEditor::DrawAssemblyPanel(m_editorMode, m_CPU->GetMemoryUnit(), *m_dissasembler, *m_CPU);
	UIEditor::DrawMemoryView(m_CPU->GetMemoryUnit(), *m_CPU, m_followPC);
	UIEditor::DrawMenu(m_CPU->GetMemoryUnit(), m_executeAuto, m_followPC, *m_CPU);
	UIEditor::DrawSpeedSlider(m_autoSpeed);
	UIEditor::DrawHelpMenu();

	ImGui::SFML::Render(m_window);
	m_window.display();
}

void Application::Run()
{
	Init();
	while (m_window.isOpen())
	{
		ProcessEvents();
		Update();
		RenderUI();
	}
	ImGui::SFML::Shutdown();
}

void Application::ProcessKeyStrokes(const std::optional<sf::Event>& event)
{
	if (ImGui::GetIO().WantCaptureKeyboard)
	{
		return;
	}
	const auto keyCode = event->getIf<sf::Event::KeyPressed>();

	switch (keyCode->code)
	{
	case sf::Keyboard::Key::Escape:
		m_window.close();
		break;

	case sf::Keyboard::Key::Space:
		m_CPU->Step();
		break;

	case sf::Keyboard::Key::R:
		m_CPU->GetMemoryUnit().RestoreSnapshot();
		m_CPU->Reset();
		m_followPC = true;
		m_executeAuto = false;
		break;

	// Speed Slider Controls
	case sf::Keyboard::Key::D:
	{
		m_autoSpeed = std::min(2.00f, m_autoSpeed + 0.10f);
		break;
	}
	case sf::Keyboard::Key::A:
	{
		m_autoSpeed = std::max(0.10f, m_autoSpeed - 0.10f);
		break;
	}

	default:
		break;
	}
}