#pragma once

#include "Hooks.h"

#include <typeinfo>  // typeid

#include "Settings.h"  // Settings

#include "RE/Skyrim.h"
#include "REL/Relocation.h"


namespace
{
	bool ForEachContainerEntry(RE::ContainerObject* a_entry)
	{
		if (!a_entry->obj || a_entry->obj->IsNot(RE::FormType::Weapon)) {
			return true;
		}

		auto weap = static_cast<RE::TESObjectWEAP*>(a_entry->obj);
		RE::EnchantmentItem* ench = weap->formEnchanting;
		if (!ench) {
			return true;
		}

		RE::EffectSetting* effectSetting = 0;
		for (auto& effect : ench->effects) {
			if (!effect) {
				continue;
			}
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
		return true;
	}


	void RemoveGlowFX(RE::Actor* a_actor)
	{
		auto container = a_actor->GetContainer();
		if (container) {
			container->ForEachContainerObject(ForEachContainerEntry);
		}

		auto changes = a_actor->GetInventoryChanges();
		if (!changes || !changes->entryList) {
			return;
		}

		for (auto& entry : *changes->entryList) {
			if (!entry->GetObject() || !entry->extraLists) {
				continue;
			}
			for (auto& xList : *entry->extraLists) {
				if (!xList || !xList->HasType(RE::ExtraDataType::kWorn)) {
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
					if (!effect) {
						continue;
					}
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


	class ActorEx : public RE::Actor
	{
	public:
		using func_t = decltype(&RE::Actor::LoadGame);
		static inline REL::Function<func_t> func;


		void Hook_LoadBuffer(RE::BGSLoadFormBuffer* a_buf)  // 0F
		{
			func(this, a_buf);
			RemoveGlowFX(this);
		}


		static void InstallHooks()
		{
			if (!*Settings::disableForWeapons) {
				REL::Offset<std::uintptr_t> vTable(REL::ID(34819));
				func = vTable.WriteVFunc(0xF, &RE::Actor::LoadGame);
				_DMESSAGE("Installed hooks for (%s)", typeid(ActorEx).name());
			}
		}
	};
}


class PlayerCharacterEx : public RE::PlayerCharacter
{
public:
	using func_t = decltype(&RE::PlayerCharacter::LoadGame);
	static inline REL::Function<func_t> func;


	void Hook_LoadBuffer(RE::BGSLoadFormBuffer* a_buf)  // 0F
	{
		func(this, a_buf);
		RemoveGlowFX(this);
	}


	static void InstallHooks()
	{
		if (!*Settings::disableForWeapons) {
			REL::Offset<std::uintptr_t> vTable(REL::ID(34819));
			func = vTable.WriteVFunc(0xF, &RE::PlayerCharacter::LoadGame);
			_DMESSAGE("Installed hooks for (%s)", typeid(PlayerCharacterEx).name());
		}
	}
};


void InstallHooks()
{
	ActorEx::InstallHooks();
	PlayerCharacterEx::InstallHooks();
}
