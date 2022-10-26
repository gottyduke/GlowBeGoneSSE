#include "Events.h"

namespace Events
{
	void DelayedWeaponTaskDelegate::Run()
	{
		auto refr = _objRefHandle.get();
		if (!refr || refr->IsNot(RE::FormType::ActorCharacter)) {
			return;
		}

		auto* actor = refr->As<RE::Actor>();
		auto* changes = actor->GetInventoryChanges();
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

				auto* weap = static_cast<RE::TESObjectWEAP*>(entry->object);
				auto* ench = weap->formEnchanting;
				if (!ench) {
					auto* xEnch = xList->GetByType<RE::ExtraEnchantment>();
					if (!xEnch || !xEnch->enchantment) {
						break;
					}

					ench = xEnch->enchantment;
				}

				for (auto& effect : ench->effects) {
					if (auto* effectSetting = effect->baseEffect; effectSetting) {
						RemoveShader(effectSetting);
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
		if (!actor) {
			return;
		}

		auto* effects = actor->AsMagicTarget()->GetActiveEffectList();
		if (!effects) {
			return;
		}

		for (auto* effect : *effects) {
			if (auto* effectSetting = effect->GetBaseObject(); effectSetting && effectSetting->formID == _formID) {
				RemoveShader(effectSetting);

				break;
			}
		}
	}

	auto TESEquipEventHandler::ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource)
		-> EventResult
	{
		if (!a_event || !a_event->actor || a_event->actor->IsNot(RE::FormType::ActorCharacter)) {
			return EventResult::kContinue;
		}

		if (auto* form = RE::TESForm::LookupByID(a_event->baseObject); !form || form->IsNot(RE::FormType::Weapon)) {
			return EventResult::kContinue;
		}

		if (Config::IsExcluded(a_event->baseObject)) {
			return EventResult::kContinue;
		}

		auto refHandle = a_event->actor->GetHandle();
		SKSE::GetTaskInterface()->AddTask(new DelayedWeaponTaskDelegate(refHandle, a_event->baseObject));

		return EventResult::kContinue;
	}

	auto TESMagicEffectApplyEventHandler::ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource)
		-> EventResult
	{
		if (!a_event || !a_event->target || !a_event->target->GetBaseObject()) {
			return EventResult::kContinue;
		}

		if (a_event->target->GetBaseObject()->Is(RE::FormType::NPC)) {
			auto* npc = static_cast<RE::TESNPC*>(a_event->target->GetBaseObject());
			if (!npc || npc->IsGhost()) {
				return EventResult::kContinue;
			}

			if (a_event->target->IsNot(RE::FormType::ActorCharacter)) {
				return EventResult::kContinue;
			}

			if (Config::IsExcluded(a_event->magicEffect)) {
				return EventResult::kContinue;
			}

			auto refHandle = a_event->target->GetHandle();
			SKSE::GetTaskInterface()->AddTask(new DelayedActorTaskDelegate(refHandle, a_event->magicEffect));
		}

		return EventResult::kContinue;
	}
}