#include "Events.h"

#include "SKSE/API.h"
#include "RE/Skyrim.h"


DelayedWeaponTaskDelegate::DelayedWeaponTaskDelegate(RE::ObjectRefHandle a_objRefHandle, UInt32 a_formID) :
	_objRefHandle(a_objRefHandle),
	_formID(a_formID)
{}


void DelayedWeaponTaskDelegate::Run()
{
	auto refr = _objRefHandle.get();
	if (!refr || refr->IsNot(RE::FormType::ActorCharacter)) {
		return;
	}

	auto actor = refr->As<RE::Actor>();
	auto changes = actor->GetInventoryChanges();
	if (!changes || !changes->entryList) {
		return;
	}

	for (auto& entry : *changes->entryList) {
		if (!entry->GetObject() || entry->GetObject()->GetFormID() != _formID || !entry->extraLists) {
			continue;
		}
		for (auto& xList : *entry->extraLists) {
			if (!xList->HasType(RE::ExtraDataType::kWorn)) {
				continue;
			}
			auto weap = static_cast<RE::TESObjectWEAP*>(entry->GetObject());
			auto ench = weap->formEnchanting;
			if (!ench) {
				auto xEnch = xList->GetByType<RE::ExtraEnchantment>();
				if (!xEnch || !xEnch->enchantment) {
					break;
				}
				ench = xEnch->enchantment;
			}
			RE::EffectSetting* effectSetting = 0;
			for (auto& effect : ench->effects) {
				if (effectSetting = effect->baseEffect) {
					RE::TESEffectShader* effectShader = 0;
					if (effectShader = effectSetting->data.effectShader) {
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


DelayedActorTaskDelegate::DelayedActorTaskDelegate(RE::ObjectRefHandle a_objRefHandle, UInt32 a_formID) :
	_objRefHandle(a_objRefHandle),
	_formID(a_formID)
{}


void DelayedActorTaskDelegate::Run()
{
	auto refr = _objRefHandle.get();
	if (!refr || refr->IsNot(RE::FormType::ActorCharacter)) {
		return;
	}

	auto actor = refr->As<RE::Actor>();
	auto effects = actor->GetActiveEffectList();
	if (!effects) {
		return;
	}

	RE::EffectSetting* effectSetting = 0;
	for (auto& effect : *effects) {
		effectSetting = effect->GetBaseObject();
		if (effectSetting && effectSetting->formID == _formID) {
			RE::TESEffectShader* effectShader = 0;
			if (effectShader = effectSetting->data.effectShader) {
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


auto TESMagicEffectApplyEventHandler::ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource)
	-> EventResult
{
	if (!a_event || !a_event->target || !a_event->target->GetBaseObject()) {
		return EventResult::kContinue;
	}

	switch (a_event->target->GetBaseObject()->GetFormType()) {
	case RE::FormType::NPC:
		{
			auto npc = static_cast<RE::TESNPC*>(a_event->target->GetBaseObject());
			if (npc->IsGhost()) {
				return EventResult::kContinue;
			}
			if (a_event->target->IsNot(RE::FormType::ActorCharacter)) {
				return EventResult::kContinue;
			}
			auto refHandle = a_event->target->CreateRefHandle();
			SKSE::GetTaskInterface()->AddTask(new DelayedActorTaskDelegate(refHandle, a_event->magicEffect));
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


auto TESEquipEventHandler::ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource)
	-> EventResult
{
	if (!a_event || !a_event->actor || a_event->actor->IsNot(RE::FormType::ActorCharacter)) {
		return EventResult::kContinue;
	}

	auto form = RE::TESForm::LookupByID(a_event->baseObject);
	if (!form || form->IsNot(RE::FormType::Weapon)) {
		return EventResult::kContinue;
	}

	auto refHandle = a_event->actor->CreateRefHandle();
	SKSE::GetTaskInterface()->AddTask(new DelayedWeaponTaskDelegate(refHandle, a_event->baseObject));

	return EventResult::kContinue;
}
