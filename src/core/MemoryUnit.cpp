#include "Core/MemoryUnit.h"

MemoryUnit::MemoryUnit()
{
	std::fill(m_memory.begin(), m_memory.end(), 0x00);
	m_hasData = false;
}

const std::array<uint8_t, 65536>& MemoryUnit::GetMemory() const noexcept
{
	return m_memory;
}

bool MemoryUnit::IsMemoryEmpty() const noexcept
{
	return !m_hasData;
}

uint8_t MemoryUnit::Read(uint16_t address) const noexcept
{
	return m_memory[address];
}

void MemoryUnit::Write(uint16_t address, uint8_t value) noexcept
{
	m_memory[address] = value;
	if (value != 0x00)
	{
		m_hasData = true;
	}
}

void MemoryUnit::Clear() noexcept
{
	std::fill(m_memory.begin(), m_memory.end(), 0x00);
	m_hasData = false;
}

void MemoryUnit::LoadValuesIntoMemory(std::span<const uint8_t> values, uint16_t startAddress)
{
	if (startAddress + values.size() > m_memory.size()) return;

	std::copy(values.begin(), values.end(), m_memory.begin() + startAddress);
	std::copy(values.begin(), values.end(), m_cache.begin());
}

void MemoryUnit::LoadProgramFromFIle(const std::filesystem::path& filename)
{
	const std::filesystem::path fullPath = std::filesystem::path("resources") / filename;
	LoadValuesIntoMemory(ProgramLoader::ParseHexValues(fullPath, m_hasData), 0x0000);
}
