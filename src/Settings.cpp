#include "Settings.h"


<<<<<<< Updated upstream
bool Settings::loadSettings(bool a_dumpParse)
{
	return Json2Settings::Settings::loadSettings(FILE_NAME, a_dumpParse);
}


decltype(Settings::disableForActors)	Settings::disableForActors("disableForActors", false);
decltype(Settings::disableForWeapons)	Settings::disableForWeapons("disableForWeapons", false);
=======
bool Settings::LoadSettings(const bool a_dumpParse)
{
	auto [log, success] = Json2Settings::load_settings(FILE_NAME, a_dumpParse);
	if (!log.empty()) {
		_ERROR("%s", log.c_str());
	}

	if (*excludePlugin == true && (*exclusionList).empty()) {
		_MESSAGE("None valid exclude plugin names detected");
		*excludePlugin = false;
	}
	for (auto& ex : *exclusionList)
		_MESSAGE("Exclusion list: %s", ex.c_str());

	return success;
}

decltype(Settings::removeActorFX) Settings::removeActorFX("removeActorFX", true);
decltype(Settings::removeWeaponFX) Settings::removeWeaponFX("removeWeaponFX", true);
decltype(Settings::excludePlugin) Settings::excludePlugin("excludePlugin", false);
decltype(Settings::exclusionList) Settings::exclusionList("exclusionList");
>>>>>>> Stashed changes
