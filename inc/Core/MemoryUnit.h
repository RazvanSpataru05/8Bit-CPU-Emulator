#pragma once

#include "IO/ProgramLoader.h"

#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <span>

class MemoryUnit
{
public:
	explicit MemoryUnit();

	[[nodiscard]] const std::array<uint8_t, 65536>& GetMemory() const noexcept;
	[[nodiscard]] bool IsMemoryEmpty() const noexcept;
	[[nodiscard]] uint8_t Read(uint16_t address) const noexcept;

	void Write(uint16_t address, uint8_t value) noexcept;
	void Clear() noexcept;
	void Load(const std::vector<uint8_t>& values, uint16_t startAddress);
	void ParseValues(const std::string& filename);

private:
	std::array<uint8_t, 65536> m_memory;
	bool m_hasData;
};

