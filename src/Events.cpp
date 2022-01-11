#include "Events.h"


namespace Events
{
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
			if (!entry->object || entry->object->GetFormID() != _formID || !entry->extraLists) {
				continue;
			}
			for (auto& xList : *entry->extraLists) {
				if (!xList->HasType(RE::ExtraDataType::kWorn)) {
					continue;
				}
				auto weap = static_cast<RE::TESObjectWEAP*>(entry->object);
				auto ench = weap->formEnchanting;
				if (!ench) {
					auto xEnch = xList->GetByType<RE::ExtraEnchantment>();
					if (!xEnch || !xEnch->enchantment) {
						break;
					}
					ench = xEnch->enchantment;
				}

				for (auto& effect : ench->effects) {
					if (auto* effectSetting = effect->baseEffect; effectSetting) {
						RE::TESEffectShader* effectShader = effectSetting->data.effectShader;

						if (effectShader) {
							effectShader->data.fillTextureEffectPersistentAlphaRatio = 0.0;
							effectShader->data.fillTextureEffectFullAlphaRatio = 0.0;
							effectShader->data.edgeEffectPersistentAlphaRatio = 0.0;
							effectShader->data.edgeEffectFullAlphaRatio = 0.0;
						}
						if (effectShader = effectSetting->data.enchantShader; effectShader) {
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


	void DelayedActorTaskDelegate::Run()
	{
		auto refr = _objRefHandle.get();
		if (!refr || refr->IsNot(RE::FormType::ActorCharacter)) {
			return;
		}

		auto* actor = refr->As<RE::Actor>();
		auto* effects = actor->GetActiveEffectList();
		if (!effects) {
			return;
		}

		for (auto* effect : *effects) {
			if (auto* effectSetting = effect->GetBaseObject(); effectSetting && effectSetting->formID == _formID) {
				RE::TESEffectShader* effectShader = effectSetting->data.effectShader;

				if (effectShader) {
					effectShader->data.fillTextureEffectPersistentAlphaRatio = 0.0;
					effectShader->data.fillTextureEffectFullAlphaRatio = 0.0;
					effectShader->data.edgeEffectPersistentAlphaRatio = 0.0;
					effectShader->data.edgeEffectFullAlphaRatio = 0.0;
				}
				
				if (effectShader = effectSetting->data.enchantShader; effectShader) {
					effectShader->data.fillTextureEffectPersistentAlphaRatio = 0.0;
					effectShader->data.fillTextureEffectFullAlphaRatio = 0.0;
					effectShader->data.edgeEffectPersistentAlphaRatio = 0.0;
					effectShader->data.edgeEffectFullAlphaRatio = 0.0;
				}
				break;
			}
		}
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

			if (Config::IsExcluded(a_event->magicEffect)) {
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


	auto TESEquipEventHandler::ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource)
		-> EventResult
	{
		if (!a_event || !a_event->actor || a_event->actor->IsNot(RE::FormType::ActorCharacter)) {
			return EventResult::kContinue;
		}

		auto* form = RE::TESForm::LookupByID(a_event->baseObject);
		if (!form || form->IsNot(RE::FormType::Weapon)) {
			return EventResult::kContinue;
		}

		if (Config::IsExcluded(a_event->baseObject)) {
			return EventResult::kContinue;
		}

		auto refHandle = a_event->actor->CreateRefHandle();
		SKSE::GetTaskInterface()->AddTask(new DelayedWeaponTaskDelegate(refHandle, a_event->baseObject));

		return EventResult::kContinue;
	}
}