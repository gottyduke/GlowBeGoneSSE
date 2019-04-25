#include "Events.h"

#include "SKSE/API.h"
#include "RE/Skyrim.h"


DelayedWeaponTaskDelegate::DelayedWeaponTaskDelegate(RE::RefHandle a_refHandle, UInt32 a_formID) :
	_refHandle(a_refHandle),
	_formID(a_formID)
{}


void DelayedWeaponTaskDelegate::Run()
{
	RE::TESObjectREFRPtr refrPtr;
	RE::TESObjectREFR::LookupByHandle(_refHandle, refrPtr);
	auto refr = refrPtr.get();
	if (!refr || refr->IsNot(RE::FormType::ActorCharacter)) {
		return;
	}

	auto actor = static_cast<RE::Actor*>(refr);
	auto changes = actor->GetInventoryChanges();
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
			auto weap = static_cast<RE::TESObjectWEAP*>(entry->type);
			auto ench = weap->objectEffect;
			if (!ench) {
				auto xEnch = xList->GetByType<RE::ExtraEnchantment>();
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
	delete this;
}


DelayedActorTaskDelegate::DelayedActorTaskDelegate(RE::RefHandle a_refHandle, UInt32 a_formID) :
	_refHandle(a_refHandle),
	_formID(a_formID)
{}


void DelayedActorTaskDelegate::Run()
{
	RE::TESObjectREFRPtr refPtr;
	RE::TESObjectREFR::LookupByHandle(_refHandle, refPtr);
	auto refr = refPtr.get();
	if (!refr || refr->IsNot(RE::FormType::ActorCharacter)) {
		return;
	}

	auto actor = static_cast<RE::Actor*>(refr);
	auto effects = actor->GetActiveEffects();
	if (!effects) {
		return;
	}

	RE::EffectSetting* effectSetting = 0;
	for (auto& effect : *effects) {
		effectSetting = effect->GetBaseEffect();
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
	delete this;
}


TESMagicEffectApplyEventHandler* TESMagicEffectApplyEventHandler::GetSingleton()
{
	static TESMagicEffectApplyEventHandler singleton;
	return &singleton;
}


RE::EventResult TESMagicEffectApplyEventHandler::ReceiveEvent(RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource)
{
	if (!a_event || !a_event->target || !a_event->target->baseForm) {
		return EventResult::kContinue;
	}

	switch (a_event->target->baseForm->formType) {
	case RE::FormType::NPC:
		{
			auto npc = static_cast<RE::TESNPC*>(a_event->target->baseForm);
			if (npc->IsGhost()) {
				return EventResult::kContinue;
			}
			if (a_event->target->IsNot(RE::FormType::ActorCharacter)) {
				return EventResult::kContinue;
			}
			auto refHandle = a_event->target->CreateRefHandle();
			SKSE::GetTaskInterface()->AddTask(new DelayedActorTaskDelegate(refHandle, a_event->formID));
			return EventResult::kContinue;
		}
	default:
		return EventResult::kContinue;
	}
}


TESEquipEventHandler* TESEquipEventHandler::GetSingleton()
{
	static TESEquipEventHandler singleton;
	return &singleton;
}


RE::EventResult TESEquipEventHandler::ReceiveEvent(RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource)
{
	if (!a_event || !a_event->akSource || a_event->akSource->IsNot(RE::FormType::ActorCharacter)) {
		return EventResult::kContinue;
	}

	auto form = RE::TESForm::LookupByID(a_event->formID);
	if (!form || form->IsNot(RE::FormType::Weapon)) {
		return EventResult::kContinue;
	}

	auto refHandle = a_event->akSource->CreateRefHandle();
	if (refHandle != *g_invalidRefHandle) {
		SKSE::GetTaskInterface()->AddTask(new DelayedWeaponTaskDelegate(refHandle, a_event->formID));
	}

	return EventResult::kContinue;
}
