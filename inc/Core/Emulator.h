#pragma once

#include "Core/InterruptController.h"
#include "Core/CPU.h"
#include "Core/Keyboard.h"
#include "Core/Terminal.h"

class Emulator
{
public:
	Emulator();

	[[nodiscard]] const CPU& GetCPU()				const noexcept;
	CPU& GetCPU()						 			noexcept;

	InterruptController& GetInterruptController()	noexcept;
	Keyboard& GetKeyboard()							noexcept;
	Terminal& GetTerminal()							noexcept;


private:
	Emulator(const Emulator&) = delete;
	Emulator& operator=(const Emulator&) = delete;

	Emulator(Emulator&&) = delete;
	Emulator& operator=(Emulator&&) = delete;

private:
	InterruptController m_interruptController;
	CPU m_cpu;
	Keyboard m_keyboard;
	Terminal m_terminal;
};