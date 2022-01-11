#pragma once


namespace Config
{
	using namespace DKUtil::Alias;

	extern Boolean RemoveActorFX;
	extern Boolean RemoveWeaponFX;
	extern String ExclusionList;
	extern std::unordered_set<RE::FormID> ExclusionID;

	
	void Load();
	bool IsExcluded(const RE::FormID a_formID);
} // namespace Config