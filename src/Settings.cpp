#include "Settings.h"


bool Settings::loadSettings(bool a_dumpParse)
{
	return Json2Settings::Settings::loadSettings(FILE_NAME, a_dumpParse);
}


decltype(Settings::disableForActors)	Settings::disableForActors("disableForActors", false);
decltype(Settings::disableForWeapons)	Settings::disableForWeapons("disableForWeapons", false);
