#include "Core/Keyboard.h"

Keyboard::Keyboard(InterruptController& interruptController) :
	m_interruptController{ interruptController } 
{
}

void Keyboard::AddKey(uint8_t key)
{
	m_buffer.emplace(key);
}

std::optional<uint8_t> Keyboard::ConsumeKey()
{
	if (m_buffer.empty()) return std::nullopt;

	uint8_t key = m_buffer.front();
	m_buffer.pop();
	return key;
}