#pragma once

#include <unordered_set>
#include "RE/Skyrim.h"

class Exclusion 
{
public:
	using FormID = UInt32;

	Exclusion() = delete;
	
	static bool	IsExcluded(const FormID a_formID);
};