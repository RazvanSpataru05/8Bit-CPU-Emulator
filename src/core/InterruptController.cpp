#include "Core/InterruptController.h"

void InterruptController::RaiseInterrupt(uint8_t line)
{
	m_pending[line] = true;
}

std::optional<uint8_t> InterruptController::PollPendingInterrupt()
{
	for (uint8_t line = 0; line < INTERRUPT_LINE_COUNT; ++line)
	{
		if (m_pending[line])
		{
			m_pending[line] = false;
			return line;
		}
	}
	return std::nullopt;
}

void InterruptController::SetKeyboardData(uint8_t key)
{
	m_keyboardData = key;
}

uint8_t InterruptController::ReadKeyboardData() const noexcept
{
	return m_keyboardData;
}

bool InterruptController::InterruptsEnabled() const noexcept
{
	return m_interruptsEnabled;
}

void InterruptController::SetInterruptsEnabled(bool enabled)
{
	m_interruptsEnabled = enabled;
}
