#include "Core/MemoryUnit.h"

MemoryUnit::MemoryUnit() :
	m_hasData{ false },
	m_snapshotStartAddress{ 0x00 }
{
	std::fill(m_memory.begin(), m_memory.end(), 0x00);
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

void MemoryUnit::RestoreSnapshot()
{
	Clear();
	for (size_t index = 0; index < m_snapshotData.size(); ++index)
	{
		m_memory[m_snapshotStartAddress + index] = m_snapshotData[index];
		if (m_snapshotStartAddress != 0x00)
		{
			m_hasData = true;
		}
	}
}

void MemoryUnit::LoadValuesIntoMemory(std::span<const uint8_t> values, uint16_t startAddress)
{
	if (startAddress + values.size() > m_memory.size() || values.empty()) return;

	std::copy(values.begin(), values.end(), m_memory.begin() + startAddress);
	m_snapshotData.assign(values.begin(), values.end());
	m_snapshotStartAddress = startAddress;
	m_hasData = true;
}

void MemoryUnit::LoadProgramFromFIle(const std::filesystem::path& filename)
{
	const std::filesystem::path fullPath = std::filesystem::path("resources") / filename;
	LoadValuesIntoMemory(DataLoader::ParseHexValues(fullPath, m_hasData), 0x0000);
}
