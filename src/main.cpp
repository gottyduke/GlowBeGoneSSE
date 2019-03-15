#include "skse64/PluginAPI.h"  // PluginHandle, SKSEInterface, PluginInfo
#include "skse64_common/skse_version.h"  // RUNTIME_VERSION

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS

#include "Events.h"  // g_task, g_magicEffectApplyEventHandler
#include "Hooks.h"  // InstallHooks
#include "Settings.h"  // Settings
#include "version.h"  // GLOWBEGONESSE_VERSION_VERSTRING

#include "RE/ScriptEventSourceHolder.h"  // ScriptEventSourceHolder


static PluginHandle				g_pluginHandle = kPluginHandle_Invalid;
static SKSEMessagingInterface*	g_messaging = 0;


void MessageHandler(SKSEMessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSEMessagingInterface::kMessage_DataLoaded:
		{
			RE::ScriptEventSourceHolder* sourceHolder = RE::ScriptEventSourceHolder::GetSingleton();

			if (!Settings::disableForActors) {
				sourceHolder->magicEffectApplyEventSource.AddEventSink(&g_magicEffectApplyEventHandler);
				_MESSAGE("[MESSAGE] Registered magic effect apply event handler");
			}

			if (!Settings::disableForWeapons) {
				sourceHolder->equipEventSource.AddEventSink(&g_equipEventHandler);
				_MESSAGE("[MESSAGE] Registered equip event event handler");
			}
		}
		break;
	}
}


extern "C" {
	bool SKSEPlugin_Query(const SKSEInterface* a_skse, PluginInfo* a_info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\GlowBeGoneSSE.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_DebugMessage);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		_MESSAGE("GlowBeGoneSSE v%s", GLOWBEGONESSE_VERSION_VERSTRING);

		a_info->infoVersion = PluginInfo::kInfoVersion;
		a_info->name = "GlowBeGoneSSE";
		a_info->version = 1;

		g_pluginHandle = a_skse->GetPluginHandle();

		if (a_skse->isEditor) {
			_FATALERROR("[FATAL ERROR] Loaded in editor, marking as incompatible!\n");
			return false;
		}

		if (a_skse->runtimeVersion != RUNTIME_VERSION_1_5_73) {
			_FATALERROR("[FATAL ERROR] Unsupported runtime version %08X!\n", a_skse->runtimeVersion);
			return false;
		}

		return true;
	}


	bool SKSEPlugin_Load(const SKSEInterface* a_skse)
	{
		_MESSAGE("[MESSAGE] GlowBeGoneSSE loaded");

		g_task = (SKSETaskInterface*)a_skse->QueryInterface(kInterface_Task);
		if (g_task) {
			_MESSAGE("[MESSAGE] Task interface query successful");
		} else {
			_FATALERROR("[FATAL ERROR] Task interface query failed!\n");
			return false;
		}

		g_messaging = (SKSEMessagingInterface*)a_skse->QueryInterface(kInterface_Messaging);
		if (g_messaging->RegisterListener(g_pluginHandle, "SKSE", MessageHandler)) {
			_MESSAGE("[MESSAGE] Messaging interface registration successful");
		} else {
			_FATALERROR("[FATAL ERROR] Messaging interface registration failed!\n");
			return false;
		}

		if (Settings::loadSettings()) {
			_MESSAGE("[MESSAGE] Settings successfully loaded");
		} else {
			_FATALERROR("[FATAL ERROR] Settings failed to load!\n");
			return false;
		}

		InstallHooks();

		return true;
	}
};
