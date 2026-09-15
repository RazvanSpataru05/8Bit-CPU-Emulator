#pragma once
#include "MemoryUnit.h"
#include <string>

class CPU
{
public:
	CPU(MemoryUnit& m_memoryUnit);

	void Step();
	void Reset();

	uint16_t ComputeAddress(uint16_t programCounter);

	//Registry getters
	[[nodiscard]] uint8_t GetA() const noexcept;
	[[nodiscard]] uint8_t GetB() const noexcept;
	[[nodiscard]] uint8_t GetC() const noexcept;
	[[nodiscard]] uint8_t GetD() const noexcept;

	[[nodiscard]] uint16_t GetPC() const noexcept;
	[[nodiscard]] uint8_t GetSP() const noexcept;
	[[nodiscard]] uint8_t GetIR() const noexcept;

	//Flag getters
	[[nodiscard]] bool GetZeroFlag() const noexcept;
	[[nodiscard]] bool GetHaltFlag() const noexcept;
	[[nodiscard]] bool GetCarryFlag() const noexcept;
	[[nodiscard]] bool GetNegativeFlag() const noexcept;
	[[nodiscard]] bool GetOverflowFlag() const noexcept;

	[[nodiscard]] bool IsReadingInstruction() const noexcept;
	[[nodiscard]] bool IsWritingInstruction() const noexcept;

private:
	[[nodiscard]] uint8_t ReadRegister(uint8_t selector) const noexcept;
	void WriteRegister(uint8_t selector, uint8_t value);

	void UpdateFlags(uint8_t result);
	void UpdateFlags(uint8_t result, uint8_t a, uint8_t opearand, bool isAdd);

private:

	// General registry
	uint8_t m_A;
	uint8_t m_B;
	uint8_t m_C;
	uint8_t m_D;

	uint8_t m_SP; // Stack Pointer

	uint16_t m_PC; // Program Counter registry
	uint8_t m_IR; // Instruction registry

	bool m_ZeroFlag;
	bool m_CarryFlag;
	bool m_NegativeFlag;
	bool m_OverflowFlag;
	bool m_HaltFlag;
	MemoryUnit& m_memoryUnit;
};

