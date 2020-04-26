#include "Events.h"
<<<<<<< Updated upstream
=======

#include <utility>
#include "Exclusion.h"
>>>>>>> Stashed changes

#include "SKSE/API.h"
#include "RE/Skyrim.h"


<<<<<<< Updated upstream
DelayedWeaponTaskDelegate::DelayedWeaponTaskDelegate(RE::RefHandle a_refHandle, UInt32 a_formID) :
	_refHandle(a_refHandle),
=======
DelayedWeaponTaskDelegate::DelayedWeaponTaskDelegate(RE::ObjectRefHandle a_objRefHandle, const UInt32 a_formID) :
	_objRefHandle(std::move(a_objRefHandle)),
>>>>>>> Stashed changes
	_formID(a_formID)
{}


void DelayedWeaponTaskDelegate::Run()
{
<<<<<<< Updated upstream
	RE::TESObjectREFRPtr refrPtr;
	RE::TESObjectREFR::LookupByHandle(_refHandle, refrPtr);
	auto refr = refrPtr.get();
=======
	const auto refr = _objRefHandle.get();
>>>>>>> Stashed changes
	if (!refr || refr->IsNot(RE::FormType::ActorCharacter)) {
		return;
	}

<<<<<<< Updated upstream
	auto actor = static_cast<RE::Actor*>(refr);
	auto changes = actor->GetInventoryChanges();
=======
	auto actor = static_cast<RE::Actor*>(refr.get());
	const auto changes = actor->GetInventoryChanges();
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
			auto weap = static_cast<RE::TESObjectWEAP*>(entry->type);
			auto ench = weap->objectEffect;
			if (!ench) {
				auto xEnch = xList->GetByType<RE::ExtraEnchantment>();
				if (!xEnch || !xEnch->objectEffect) {
=======
			const auto weap = static_cast<RE::TESObjectWEAP*>(entry->GetObject());
			auto ench = weap->formEnchanting;
			if (!ench) {
				const auto xEnch = xList->GetByType<RE::ExtraEnchantment>();
				if (!xEnch || !xEnch->enchantment) {
>>>>>>> Stashed changes
					break;
				}
				ench = xEnch->objectEffect;
			}
			RE::EffectSetting* effectSetting;
			for (auto& effect : ench->effects) {
<<<<<<< Updated upstream
				if (effectSetting = effect->baseEffect) {
					RE::TESEffectShader* effectShader = 0;
					if (effectShader = effectSetting->data.hitShader) {
=======
				if ((effectSetting = effect->baseEffect)) {
					RE::TESEffectShader* effectShader;
					if ((effectShader = effectSetting->data.effectShader)) {
>>>>>>> Stashed changes
						effectShader->data.fillTextureEffectPersistentAlphaRatio = 0.0;
						effectShader->data.fillTextureEffectFullAlphaRatio = 0.0;
						effectShader->data.edgeEffectPersistentAlphaRatio = 0.0;
						effectShader->data.edgeEffectFullAlphaRatio = 0.0;
					}
					if ((effectShader = effectSetting->data.enchantShader)) {
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


<<<<<<< Updated upstream
DelayedActorTaskDelegate::DelayedActorTaskDelegate(RE::RefHandle a_refHandle, UInt32 a_formID) :
	_refHandle(a_refHandle),
=======
DelayedActorTaskDelegate::DelayedActorTaskDelegate(RE::ObjectRefHandle a_objRefHandle, const UInt32 a_formID) :
	_objRefHandle(std::move(a_objRefHandle)),
>>>>>>> Stashed changes
	_formID(a_formID)
{}


void DelayedActorTaskDelegate::Run()
{
<<<<<<< Updated upstream
	RE::TESObjectREFRPtr refPtr;
	RE::TESObjectREFR::LookupByHandle(_refHandle, refPtr);
	auto refr = refPtr.get();
=======
	const auto refr = _objRefHandle.get();
>>>>>>> Stashed changes
	if (!refr || refr->IsNot(RE::FormType::ActorCharacter)) {
		return;
	}

<<<<<<< Updated upstream
	auto actor = static_cast<RE::Actor*>(refr);
	auto effects = actor->GetActiveEffects();
=======
	auto actor = static_cast<RE::Actor*>(refr.get());
	const auto effects = actor->GetActiveEffectList();
>>>>>>> Stashed changes
	if (!effects) {
		return;
	}

	for (auto& effect : *effects) {
<<<<<<< Updated upstream
		effectSetting = effect->GetBaseEffect();
		if (effectSetting && effectSetting->formID == _formID) {
			RE::TESEffectShader* effectShader = 0;
			if (effectShader = effectSetting->data.hitShader) {
=======
		const auto effectSetting = effect->GetBaseObject();
		if (effectSetting && effectSetting->formID == _formID) {
			RE::TESEffectShader* effectShader;
			if ((effectShader = effectSetting->data.effectShader)) {
>>>>>>> Stashed changes
				effectShader->data.fillTextureEffectPersistentAlphaRatio = 0.0;
				effectShader->data.fillTextureEffectFullAlphaRatio = 0.0;
				effectShader->data.edgeEffectPersistentAlphaRatio = 0.0;
				effectShader->data.edgeEffectFullAlphaRatio = 0.0;
			}
			if ((effectShader = effectSetting->data.enchantShader)) {
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
	return std::addressof(singleton);
}


<<<<<<< Updated upstream
RE::EventResult TESMagicEffectApplyEventHandler::ReceiveEvent(RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource)
=======
auto TESMagicEffectApplyEventHandler::ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event,
                                                   RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource)
-> EventResult
>>>>>>> Stashed changes
{
	if (!a_event || !a_event->target || !a_event->target->baseForm) {
		return EventResult::kContinue;
	}

	switch (a_event->target->baseForm->formType) {
	case RE::FormType::NPC:
		{
<<<<<<< Updated upstream
			auto npc = static_cast<RE::TESNPC*>(a_event->target->baseForm);
=======
			const auto npc = static_cast<RE::TESNPC*>(a_event->target->GetBaseObject());
>>>>>>> Stashed changes
			if (npc->IsGhost()) {
				return EventResult::kContinue;
			}
			
			if (a_event->target->IsNot(RE::FormType::ActorCharacter)) {
				return EventResult::kContinue;
			}
<<<<<<< Updated upstream
			auto refHandle = a_event->target->CreateRefHandle();
			SKSE::GetTaskInterface()->AddTask(new DelayedActorTaskDelegate(refHandle, a_event->formID));
=======

			if (*Settings::excludePlugin && Exclusion::IsExcluded(a_event->magicEffect)) {
				return EventResult::kContinue;
			}

			const auto refHandle = a_event->target->CreateRefHandle();
			SKSE::GetTaskInterface()->AddTask(new DelayedActorTaskDelegate(refHandle, a_event->magicEffect));
>>>>>>> Stashed changes
			return EventResult::kContinue;
		}
	default:
		return EventResult::kContinue;
	}
}


TESEquipEventHandler* TESEquipEventHandler::GetSingleton()
{
	static TESEquipEventHandler singleton;
	return std::addressof(singleton);
}


<<<<<<< Updated upstream
RE::EventResult TESEquipEventHandler::ReceiveEvent(RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource)
{
	if (!a_event || !a_event->akSource || a_event->akSource->IsNot(RE::FormType::ActorCharacter)) {
		return EventResult::kContinue;
	}

	auto form = RE::TESForm::LookupByID(a_event->formID);
=======
auto TESEquipEventHandler::ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource)
-> EventResult
{
	if (!*Settings::removeWeaponFX) {
		return EventResult::kContinue;
	}
	
	if (!a_event || !a_event->actor || a_event->actor->IsNot(RE::FormType::ActorCharacter)) {
		return EventResult::kContinue;
	}

	const auto form = RE::TESForm::LookupByID(a_event->baseObject);
>>>>>>> Stashed changes
	if (!form || form->IsNot(RE::FormType::Weapon)) {
		return EventResult::kContinue;
	}

	auto refHandle = a_event->akSource->CreateRefHandle();
	if (refHandle != *g_invalidRefHandle) {
		SKSE::GetTaskInterface()->AddTask(new DelayedWeaponTaskDelegate(refHandle, a_event->formID));
	}

<<<<<<< Updated upstream
=======
	const auto objRefHandle = a_event->actor->CreateRefHandle();
	SKSE::GetTaskInterface()->AddTask(new DelayedWeaponTaskDelegate(objRefHandle, a_event->baseObject));

>>>>>>> Stashed changes
	return EventResult::kContinue;
}
