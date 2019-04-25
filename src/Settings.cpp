#include "Settings.h"


bool Settings::loadSettings(bool a_dumpParse)
{
	setFileName(FILE_NAME);
	return Json2Settings::Settings::loadSettings(a_dumpParse);
}


decltype(Settings::disableForActors)	Settings::disableForActors("disableForActors", true, false);
decltype(Settings::disableForWeapons)	Settings::disableForWeapons("disableForWeapons", true, false);
