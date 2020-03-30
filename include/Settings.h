#pragma once

#include "Json2Settings.h"

namespace J2S = Json2Settings;

class Settings
{
public:

	Settings() = delete;

	static bool	LoadSettings(bool a_dumpParse = false);

	static J2S::bSetting				removeActorFX;
	static J2S::bSetting				removeWeaponFX;
	//static J2S::bSetting				removeArmorFX;
	static J2S::bSetting				excludePlugin;
	static J2S::aSetting<std::string>	exclusionList;

private:
	static constexpr char* FILE_NAME = "Data\\SKSE\\Plugins\\GlowBeGoneSSE.json";
};
