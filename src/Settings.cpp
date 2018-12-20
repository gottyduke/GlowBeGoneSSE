#include "Settings.h"


bool Settings::loadSettings(bool a_dumpParse)
{
	setFileName(FILE_NAME);
	return Json2Settings::Settings::loadSettings(a_dumpParse);
}


bSetting	Settings::disableForActors("disableForActors", true, false);
bSetting	Settings::disableForWeapons("disableForWeapons", true, false);
