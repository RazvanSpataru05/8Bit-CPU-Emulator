#include "Assembler/ISAEntry.h"

namespace ISA
{
	const ISAEntry* Find(const std::string& mnemonic)
	{
		auto it = s_opcodeTable.find(mnemonic);
		return it != s_opcodeTable.end() ? it->second : nullptr;
	}
	const ISAEntry* GetISATable(int& outSize)
	{
		outSize = sizeof(ISATable) / sizeof(ISAEntry);
		return ISATable;
	}

	bool IsMnemonic(const std::string& word)
	{
		return Find(word) != nullptr;
	}
}
