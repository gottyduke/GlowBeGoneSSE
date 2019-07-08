#include "Hooks.h"

#include "skse64_common/SafeWrite.h"  // SafeWrit64

#include <typeinfo>  // typeid

#include "Settings.h"  // Settings

#include "RE/Skyrim.h"
#include "REL/Relocation.h"


namespace
{
	bool ForEachContainerEntry(RE::TESContainer::Entry* a_entry)
	{
		if (!a_entry->form || a_entry->form->IsNot(RE::FormType::Weapon)) {
			return true;
		}

		auto weap = static_cast<RE::TESObjectWEAP*>(a_entry->form);
		RE::EnchantmentItem* ench = weap->objectEffect;
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
		return true;
	}


	void RemoveGlowFX(RE::Actor* a_actor)
	{
		auto container = a_actor->GetContainer();
		if (container) {
			container->ForEach(ForEachContainerEntry);
		}

		auto changes = a_actor->GetInventoryChanges();
		if (!changes || !changes->entryList) {
			return;
		}

		for (auto& entry : *changes->entryList) {
			if (!entry->type || !entry->extraList) {
				continue;
			}
			for (auto& xList : *entry->extraList) {
				if (!xList || !xList->HasType(RE::ExtraDataType::kWorn)) {
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
					if (!effect) {
						continue;
					}
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


	class ActorEx : public RE::Actor
	{
	public:
		using func_t = function_type_t<decltype(&RE::Actor::LoadBuffer)>;
		inline static func_t* func;


		void Hook_LoadBuffer(RE::BGSLoadFormBuffer* a_buf)
		{
			func(this, a_buf);
			RemoveGlowFX(this);
		}


		static void InstallHooks()
		{
			if (!Settings::disableForWeapons) {
				REL::Offset<func_t**> vFunc(RE::Offset::Actor::Vtbl + (0xF * 0x8));
				func = *vFunc;
				SafeWrite64(vFunc.GetAddress(), GetFnAddr(&Hook_LoadBuffer));
				_DMESSAGE("Installed hooks for (%s)", typeid(ActorEx).name());
			}
		}
	};
}


class PlayerCharacterEx : public RE::PlayerCharacter
{
public:
	using func_t = function_type_t<decltype(&RE::PlayerCharacter::LoadBuffer)>;
	inline static func_t* func;


	void Hook_LoadBuffer(RE::BGSLoadFormBuffer* a_buf)
	{
		func(this, a_buf);
		RemoveGlowFX(this);
	}


	static void InstallHooks()
	{
		if (!Settings::disableForWeapons) {
			REL::Offset<func_t**> vFunc(RE::Offset::PlayerCharacter::Vtbl + (0xF * 0x8));
			func = *vFunc;
			SafeWrite64(vFunc.GetAddress(), GetFnAddr(&Hook_LoadBuffer));
			_DMESSAGE("Installed hooks for (%s)", typeid(PlayerCharacterEx).name());
		}
	}
};


void InstallHooks()
{
	ActorEx::InstallHooks();
	PlayerCharacterEx::InstallHooks();
}
