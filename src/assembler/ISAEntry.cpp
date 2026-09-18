#include "Assembler/ISAEntry.h"

const ISAEntry* GetISATable(int& outSize)
{
	outSize = sizeof(ISATable) / sizeof(ISAEntry);
	return ISATable;
}

bool IsMnemonic(const std::string& word)
{
	const size_t tableSize = static_cast<size_t>(sizeof(ISATable) / sizeof(ISAEntry));
	for (size_t index = 0; index < tableSize; ++index)
	{
		if (ISATable[index].mnemonic == word)
		{
			return true;
		}
	}
	return false;
}
