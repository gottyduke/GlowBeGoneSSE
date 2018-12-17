#include "Events.h"

#include "skse64/PluginAPI.h"  // SKSETaskInterface

#include "RE/ActiveEffect.h"  // ActiveEffect
#include "RE/Actor.h"  // Actor
#include "RE/BSTEvent.h"  // EventResult, BSTEventSource
#include "RE/BSTList.h"  // BSTList
#include "RE/EffectSetting.h"  // EffectSetting
#include "RE/FormTypes.h"  // FormType
#include "RE/TESEffectShader.h"  // TESEffectShader
#include "RE/TESForm.h"  // TESForm
#include "RE/TESMagicEffectApplyEvent.h"  // TESMagicEffectApplyEvent
#include "RE/TESNPC.h"  // TESNPC
#include "RE/TESObjectREFR.h"  // TESObjectREFR


void DelayedTaskDelegate::Run()
{
	RE::BSSimpleList<RE::ActiveEffect*>* effects = _actor->GetActiveEffects();
	if (!effects) {
		return;
	}

	RE::EffectSetting* effectSetting = 0;
	for (auto& effect : *effects) {
		effectSetting = effect->GetBaseObject();
		if (effectSetting && effectSetting->formID == _formID) {
			RE::TESEffectShader* effectShader = 0;
			if (effectShader = effectSetting->data.hitShader) {
				effectShader->data.fillTextureEffectPersistentAlphaRatio = 0.0;
				effectShader->data.fillTextureEffectFullAlphaRatio = 0.0;
				effectShader->data.edgeEffectPersistentAlphaRatio = 0.0;
				effectShader->data.edgeEffectFullAlphaRatio = 0.0;
			}
			if (effectShader = effectSetting->data.enchantShader) {
				effectShader->data.fillTextureEffectPersistentAlphaRatio = 0.0;
				effectShader->data.fillTextureEffectFullAlphaRatio = 0.0;
				effectShader->data.edgeEffectPersistentAlphaRatio = 0.0;
				effectShader->data.edgeEffectFullAlphaRatio = 0.0;
			}
			break;
		}
	}
}


void DelayedTaskDelegate::Dispose()
{
	if (this) {
		delete this;
	}
}


TESMagicEffectApplyEventHandler::~TESMagicEffectApplyEventHandler()
{}


RE::EventResult TESMagicEffectApplyEventHandler::ReceiveEvent(RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource)
{
	using RE::EventResult;

	if (!a_event || !a_event->target || a_event->target->IsNot(RE::FormType::Character)) {
		return EventResult::kContinue;
	}

	RE::Actor* actor = static_cast<RE::Actor*>(a_event->target);
	RE::TESNPC* npc = actor->GetActorBase();
	if (!npc || npc->IsGhost()) {
		return EventResult::kContinue;
	}

	if (!RE::TESForm::LookupByID<RE::EffectSetting>(a_event->formID)) {
		return EventResult::kContinue;
	}

	DelayedTaskDelegate* dlgt = new DelayedTaskDelegate(actor, a_event->formID);
	g_task->AddTask(dlgt);

	return EventResult::kContinue;
}


SKSETaskInterface* g_task = 0;
TESMagicEffectApplyEventHandler g_magicEffectApplyEventHandler;
