#include "Events.h"

#include "skse64/PluginAPI.h"  // SKSETaskInterface

#include "RE/ActiveEffect.h"  // ActiveEffect
#include "RE/Actor.h"  // Actor
#include "RE/BSTEvent.h"  // EventResult, BSTEventSource
#include "RE/BSTList.h"  // BSTList
#include "RE/EffectItem.h"  // EffectItem
#include "RE/EffectSetting.h"  // EffectSetting
#include "RE/EnchantmentItem.h"  // EnchantmentItem
#include "RE/ExtraEnchantment.h"  // ExtraEnchantment
#include "RE/FormTypes.h"  // FormType
#include "RE/InventoryChanges.h"  // InventoryChanges
#include "RE/InventoryEntryData.h"  // InventoryEntryData
#include "RE/PlayerCharacter.h"  // PlayerCharacter
#include "RE/TESEffectShader.h"  // TESEffectShader
#include "RE/TESForm.h"  // TESForm
#include "RE/TESMagicEffectApplyEvent.h"  // TESMagicEffectApplyEvent
#include "RE/TESNPC.h"  // TESNPC
#include "RE/TESObjectREFR.h"  // TESObjectREFR
#include "RE/TESObjectWEAP.h"  // TESObjectWEAP


void DelayedWeaponTaskDelegate::Run()
{
	RE::TESObjectREFRPtr refrPtr;
	RE::TESObjectREFR::LookupByHandle(_refHandle, refrPtr);
	RE::TESObjectREFR* refr = refrPtr;
	if (!refr || refr->IsNot(RE::FormType::Character)) {
		return;
	}

	RE::Actor* actor = static_cast<RE::Actor*>(refr);
	RE::InventoryChanges* changes = actor->GetInventoryChanges();
	if (!changes || !changes->entryList) {
		return;
	}

	for (auto& entry : *changes->entryList) {
		if (!entry->type || entry->type->formID != _formID || !entry->extraList) {
			continue;
		}
		for (auto& xList : *entry->extraList) {
			if (!xList->HasType(RE::ExtraDataType::kWorn)) {
				continue;
			}
			RE::TESObjectWEAP* weap = static_cast<RE::TESObjectWEAP*>(entry->type);
			RE::EnchantmentItem* ench = weap->objectEffect;
			if (!ench) {
				RE::ExtraEnchantment* xEnch = xList->GetByType<RE::ExtraEnchantment>();
				if (!xEnch || !xEnch->objectEffect) {
					break;
				}
				ench = xEnch->objectEffect;
			}
			RE::EffectSetting* effectSetting = 0;
			for (auto& effect : ench->effects) {
				if (effectSetting = effect->baseEffect) {
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
				}
			}
		}
	}
}


void DelayedWeaponTaskDelegate::Dispose()
{
	if (this) {
		delete this;
	}
}


void DelayedActorTaskDelegate::Run()
{
	RE::TESObjectREFRPtr refPtr;
	RE::TESObjectREFR::LookupByHandle(_refHandle, refPtr);
	RE::TESObjectREFR* refr = refPtr;
	if (!refr || refr->IsNot(RE::FormType::Character)) {
		return;
	}

	RE::Actor* actor = static_cast<RE::Actor*>(refr);
	RE::BSSimpleList<RE::ActiveEffect*>* effects = actor->GetActiveEffects();
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


void DelayedActorTaskDelegate::Dispose()
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

	if (!a_event || !a_event->target || !a_event->target->baseForm) {
		return EventResult::kContinue;
	}

	switch (a_event->target->baseForm->formType) {
	case RE::FormType::NPC:
	{
		RE::TESNPC* npc = static_cast<RE::TESNPC*>(a_event->target->baseForm);
		if (npc->IsGhost()) {
			return EventResult::kContinue;
		}
		if (a_event->target->IsNot(RE::FormType::Character)) {
			return EventResult::kContinue;
		}
		UInt32 refHandle = a_event->target->CreateRefHandle();
		DelayedActorTaskDelegate* dlgt = new DelayedActorTaskDelegate(refHandle, a_event->formID);
		g_task->AddTask(dlgt);
		return EventResult::kContinue;
	}
	default:
		return EventResult::kContinue;
	}
}


TESEquipEventHandler::~TESEquipEventHandler()
{}


RE::EventResult TESEquipEventHandler::ReceiveEvent(RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource)
{
	using RE::EventResult;

	if (!a_event || !a_event->akSource || a_event->akSource->IsNot(RE::FormType::Character)) {
		return EventResult::kContinue;
	}

	RE::TESForm* form = RE::TESForm::LookupByID(a_event->formID);
	if (!form || form->IsNot(RE::FormType::Weapon)) {
		return EventResult::kContinue;
	}

	UInt32 refHandle = a_event->akSource->CreateRefHandle();
	if (refHandle != *g_invalidRefHandle) {
		DelayedWeaponTaskDelegate* dlgt = new DelayedWeaponTaskDelegate(refHandle, a_event->formID);
		g_task->AddTask(dlgt);
	}

	return EventResult::kContinue;
}


SKSETaskInterface* g_task = 0;
TESMagicEffectApplyEventHandler g_magicEffectApplyEventHandler;
TESEquipEventHandler g_equipEventHandler;
