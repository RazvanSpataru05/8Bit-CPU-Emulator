#pragma once

#include "Assembler/Assembler.h"

#include "Core/CPU.h"

#include "Debugger/Dissasembler.h"

#include "UI/UIEditor.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Application
{
public:
	explicit Application();

	[[nodiscard]] const CPU& GetCPU() const;
	[[nodiscard]] const Assembler& GetAssembler() const;

	void Init();
	void ProcessEvents();
	void Update();
	void RenderUI();
	void Run();

	void ProcessKeyStrokes(const std::optional<sf::Event>& event);

private:
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	Application(Application&&) = delete;
	Application& operator=(Application&&) = delete;

private:
	std::unique_ptr<CPU> m_CPU;
	std::unique_ptr<Dissasembler> m_dissasembler;
	std::unique_ptr<Assembler> m_assembler;

	UIEditor::Mode m_editorMode;

	bool m_executeAuto;
	bool m_followPC;
	bool m_showISA;
	float m_autoSpeed;

	sf::RenderWindow m_window;
	sf::Clock m_deltaClock;
	sf::Clock m_instructionCycle;
	sf::Font m_font;
};