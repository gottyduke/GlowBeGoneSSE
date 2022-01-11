#include "Config.h"


namespace Config
{
	Boolean RemoveActorFX{ "RemoveActorFX" };
	Boolean RemoveWeaponFX{ "RemoveWeaponFX" };
	String ExclusionList{ "ExclusionList" };
	std::unordered_set<RE::FormID> ExclusionID;


	void Load()
	{
		auto mainConfig = COMPILE_PROXY("GlowBeGoneSSE.toml"sv);

		mainConfig.Bind(RemoveActorFX, true);
		mainConfig.Bind(RemoveWeaponFX, true);
		mainConfig.Bind(ExclusionList, 
			"Campfire.esm",
			"Dirt and Blood - Dynamic Visuals.esp",
			"FleshFX.esp",
			"Frostfall.esp",
			"RASS - Visual Effects.esl",
			"WetandCold.esp"
		);

		mainConfig.Load();

		INFO("Config Loaded"sv);
	}


	bool IsExcluded(const RE::FormID a_formID)
	{
		if (ExclusionID.count(a_formID)) {
			return true;
		}

		auto* dataHandler = RE::TESDataHandler::GetSingleton();
		std::string file = RE::TESForm::LookupByID(a_formID)->GetFile()->fileName;

		for (auto& ex : Config::ExclusionList.get_collection()) {
			if ((dataHandler->LookupLoadedModByName(ex) ||
				dataHandler->LookupLoadedLightModByName(ex)) &&
				ex == file) {
				ExclusionID.insert(a_formID);
				return true;
			}
		}

		return false;
	}
} // namespace Config