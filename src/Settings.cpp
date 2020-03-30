#include "Settings.h"


bool Settings::LoadSettings(bool a_dumpParse)
{
	auto [log, success] = Json2Settings::load_settings(FILE_NAME, a_dumpParse);
	if (!log.empty()) {
		_ERROR("%s", log.c_str());
	}

	if (*excludePlugin == true && (*exclusionList).empty()) {
		_MESSAGE("None valid exclude plugin names detected");
		*excludePlugin = false;
	}
	for (auto& ex : *Settings::exclusionList)
		_MESSAGE("Exclusion list: %s", ex);

	return success;
}

decltype(Settings::removeActorFX)		Settings::removeActorFX("removeActorFX", true);
decltype(Settings::removeWeaponFX)		Settings::removeWeaponFX("removeWeaponFX", true);
//decltype(Settings::removeArmorFX)		Settings::removeArmorFX("removeArmorFX", false);
decltype(Settings::excludePlugin)		Settings::excludePlugin("excludePlugin", false);
decltype(Settings::exclusionList)		Settings::exclusionList("exclusionList");