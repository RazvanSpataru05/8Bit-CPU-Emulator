#pragma once

#include "IO/DataLoader.h"

#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <span>

class MemoryUnit
{
public:
	explicit MemoryUnit();

	[[nodiscard]] bool IsMemoryEmpty() const noexcept;

	[[nodiscard]] uint8_t Read(uint16_t address) const noexcept;
	void Write(uint16_t address, uint8_t value) noexcept;

	void Clear() noexcept;
	void RestoreSnapshot();

	void LoadValuesIntoMemory(std::span<const uint8_t> program, uint16_t startAddress = 0x0000);
	void LoadProgramFromFIle(const std::filesystem::path& filename);

private:
	MemoryUnit(const MemoryUnit&) = delete;
	MemoryUnit& operator=(const MemoryUnit&) = delete;

private:
	std::array<uint8_t, 65536> m_memory;
	bool m_hasData;
	std::vector<uint8_t> m_snapshotData;
	uint16_t m_snapshotStartAddress;
};

