#pragma once

#include "Json2Settings.h"


class Settings : public Json2Settings::Settings
{
public:
	Settings() = delete;

<<<<<<< Updated upstream
	static bool	loadSettings(bool a_dumpParse = false);


	static bSetting	disableForActors;
	static bSetting	disableForWeapons;
=======
	static bool LoadSettings(bool a_dumpParse = false);

	static J2S::bSetting				removeActorFX;
	static J2S::bSetting				removeWeaponFX;
	static J2S::bSetting				excludePlugin;
	static J2S::aSetting<std::string>	exclusionList;
>>>>>>> Stashed changes

private:
	static constexpr char* FILE_NAME = "Data\\SKSE\\Plugins\\GlowBeGoneSSE.json";
};
