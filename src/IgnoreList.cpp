#include "IgnoreList.h"

namespace IgnoreList {

	using FormID = UInt32;

	std::unordered_set<FormID> FleshFX = {
		0x0051B15,		// Oakflesh
		0x0059B7A,		// Stoneflesh
		0x0059B7B,		// Ironflesh
		0x0059B7C,		// Ebonyflesh
		0x00CDB75		// Dragonhide
	};
}