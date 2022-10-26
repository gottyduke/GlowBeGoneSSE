#include "Config.h"
#include "Events.h"
#include "Hooks.h"

namespace
{
	void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
			auto* source = RE::ScriptEventSourceHolder::GetSingleton();

			if (*Config::RemoveActorFX) {
				source->AddEventSink(Events::TESMagicEffectApplyEventHandler::GetSingleton());
				INFO("Registered magic effect apply event handler"sv);
			}

			if (*Config::RemoveWeaponFX) {
				source->AddEventSink(Events::TESEquipEventHandler::GetSingleton());
				INFO("Registered equip event event handler"sv);
			}
		}
	}
}

DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	DKUtil::Logger::Init(Plugin::NAME, REL::Module::get().version().string());

	SKSE::Init(a_skse);

	INFO("{} v{} loaded", Plugin::NAME, Plugin::Version);

	// do stuff
	Config::Load();

	auto* messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener(MessageHandler)) {
		return false;
	}

	if (*Config::RemoveActorFX) {
		Hooks::Install();
	}

	return true;
}