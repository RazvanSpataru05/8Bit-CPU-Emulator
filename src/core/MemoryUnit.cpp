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

void MemoryUnit::Load(uint16_t startAddress, const std::vector<uint8_t>& values)
{
	if (startAddress + values.size() > m_memory.size()) return;

	std::copy(values.begin(), values.end(), m_memory.begin() + startAddress);
}

void MemoryUnit::ParseValues(const std::string& filename)
{
	const std::string path = "resources/" + filename;
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cout << "Error: Could not open file" << std::endl;
		return;
	}

	std::vector<uint8_t> values;
	unsigned hexValue{};
	while (file >> std::hex >> hexValue)
	{
		uint8_t value = static_cast<uint8_t>(hexValue);
		values.emplace_back(value);
		if (value != 0x00)
		{
			m_hasData = true;
		}
	}

	file.close();
	std::cout << values.size() << std::endl;
	Load(0x00, values);
}
