#pragma once

#include "Core/CPU.h"
#include "Debugger/Dissasembler.h"
#include "UI/UIEditor.h"

#include <SFML/Graphics.hpp>

class Application
{
public:
	explicit Application();

	void Run();

private:
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	Application(Application&&) = delete;
	Application& operator=(Application&&) = delete;

private:
	std::unique_ptr<CPU> m_CPU;
	std::unique_ptr<Dissasembler> m_dissasembler;

};

