#include "Assembler/ISAEntry.h"

namespace ISA
{
	const ISAEntry* Find(const std::string& mnemonic)
	{
		auto it = s_opcodeTable.find(mnemonic);
		return it != s_opcodeTable.end() ? it->second : nullptr;
	}

	size_t GetISATableSize()
	{
		return static_cast<size_t>(sizeof(Table) / sizeof(ISAEntry));
	}

	bool IsMnemonic(const std::string& word)
	{
		return Find(word) != nullptr;
	}
}
