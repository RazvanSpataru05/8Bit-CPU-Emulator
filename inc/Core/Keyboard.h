#pragma once

#include <optional>
#include <cstdint>
#include <queue>

class Keyboard
{
public:
	Keyboard() = default;

	void AddKey(uint8_t key);
	std::optional<uint8_t> ConsumeKey();

private:
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	Keyboard(Keyboard&&) = delete;
	Keyboard& operator=(Keyboard&&) = delete;

private:
	std::queue<uint8_t> m_buffer;
};