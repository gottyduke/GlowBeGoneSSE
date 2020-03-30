#include "Exclusion.h"
#include "Settings.h"

bool Exclusion::IsExcluded(const FormID a_formID)
{
	auto dataHandler = RE::TESDataHandler::GetSingleton();
	auto file = RE::TESForm::LookupByID(a_formID)->GetFile()->fileName;
	for (auto& ex : *Settings::exclusionList) {
		if ((dataHandler->LookupLoadedModByName(ex) ||
			dataHandler->LookupLoadedLightModByName(ex)) &&
			file == ex)
				return true;
	}
	return false;
}