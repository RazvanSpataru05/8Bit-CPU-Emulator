#include "Assembler/ISAEntry.h"

const ISAEntry* GetISATable(int& outSize)
{
	outSize = sizeof(isaTable) / sizeof(ISAEntry);
	return isaTable;
}

bool IsMnemonic(const std::string& word)
{
	for (size_t index = 0; index < isaTable->size; ++index)
	{
		if (isaTable->mnemonic == word)
		{
			return true;
		}
	}
	return false;
}
