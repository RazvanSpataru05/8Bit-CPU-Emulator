#pragma once

#include <cstdint>
#include <array>
#include <optional>

constexpr uint8_t INTERRUPT_LINE_COUNT{ 8u };
constexpr uint8_t KEYBOARD_INTERRUPT_LINE{ 0x00 };

class InterruptController
{

public:
	InterruptController() = default;

	InterruptController(InterruptController&&) = default;
	InterruptController& operator=(InterruptController&&) = default;

	void RaiseInterrupt(uint8_t line);
	std::optional<uint8_t> PollPendingInterrupt();

	// keyboard interrupt line
	void SetKeyboardData(uint8_t key);
	uint8_t ReadKeyboardData() const noexcept;

	bool GetInterruptFlag()			const noexcept;
	void EnableInterrupts(bool enabled) noexcept;

private:
	InterruptController(const InterruptController&) = delete;
	InterruptController& operator=(const InterruptController&) = delete;

private:
	std::array<bool, INTERRUPT_LINE_COUNT> m_pending{};

	uint8_t m_keyboardData{};
	bool m_interruptFlag{ false };
};