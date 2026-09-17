#pragma once

#include "Core/CPU.h"
#include "Debugger/Dissasembler.h"
#include "UI/UIEditor.h"

#include <SFML/Graphics.hpp>

class Application
{
public:
	explicit Application();

	[[nodiscard]] const CPU& GetCPU() const;

	void Init();
	void Run();

private:
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	Application(Application&&) = delete;
	Application& operator=(Application&&) = delete;

private:
	std::unique_ptr<CPU> m_CPU;
	std::unique_ptr<Dissasembler> m_dissasembler;

	EditorMode m_editorMode;

	bool m_executeAuto;
	bool m_followPC;
	bool m_showISA;
	float m_autoSpeed;

	sf::RenderWindow m_window;
	sf::Clock m_deltaClock;
};

