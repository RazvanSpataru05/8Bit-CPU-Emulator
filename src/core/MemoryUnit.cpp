#include "Core/MemoryUnit.h"

MemoryUnit::MemoryUnit() :
	m_hasData{ false },
	m_startAddress{ 0x0000 },
	m_endAddress{ 0x0000 }
{
	std::fill(m_memory.begin(), m_memory.end(), 0x00);
}

bool MemoryUnit::IsMemoryEmpty() const noexcept
{
	return !m_hasData;
}
uint16_t MemoryUnit::GetStartAddress() const noexcept
{
	return m_startAddress;
}

uint16_t MemoryUnit::GetEndAddress() const noexcept
{
	return m_endAddress;
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
		m_memory[m_startAddress + index] = m_snapshotData[index];
		if (m_snapshotData[index] != 0x00)
		{
			m_hasData = true;
		}
	}
	m_endAddress = m_startAddress + m_snapshotData.size();
}

void MemoryUnit::PrintMemoryUntit() const
{
	for (uint8_t opcode : m_memory)
	{
		std::cout << std::hex << static_cast<int>(opcode) << " ";
	}
}

void MemoryUnit::LoadValuesIntoMemory(std::span<const uint8_t> values, uint16_t startAddress)
{
	if (startAddress + values.size() > m_memory.size() || values.empty()) return;

	std::copy(values.begin(), values.end(), m_memory.begin() + startAddress);
	m_snapshotData.assign(values.begin(), values.end());
	m_startAddress = startAddress;
	m_endAddress = startAddress + values.size();
	m_hasData = true;
}

void MemoryUnit::LoadProgramFromFIle(const std::filesystem::path& filename)
{
	const std::filesystem::path fullPath = std::filesystem::path("resources") / filename;
	LoadValuesIntoMemory(DataLoader::ParseHexValues(fullPath, m_hasData), 0x0000);
}

const uint8_t& MemoryUnit::operator[](uint16_t address) const
{
	return m_memory[address];
}

uint8_t& MemoryUnit::operator[](uint16_t address)
{
	return m_memory[address];
}
