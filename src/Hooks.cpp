#include "Hooks.h"

#include "skse64_common/Relocation.h"  // RelocPtr
#include "skse64_common/SafeWrite.h"  // SafeWrit64

#include <typeinfo>  // typeid

#include "Settings.h"  // Settings

#include "RE/Skyrim.h"


class TESContainerVisitor
{
public:
	bool Accept(RE::TESContainer::Entry* a_entry)
	{
		if (!a_entry->form || a_entry->form->IsNot(RE::FormType::Weapon)) {
			return true;
		}

		RE::TESObjectWEAP* weap = static_cast<RE::TESObjectWEAP*>(a_entry->form);
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
};


void RemoveGlowFX(RE::Actor* a_actor)
{
	RE::TESContainer* container = a_actor->GetContainer();
	if (container) {
		TESContainerVisitor visitor;
		container->Visit(visitor);
	}

	RE::InventoryChanges* changes = a_actor->GetInventoryChanges();
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
	typedef void _LoadBuffer_t(RE::Actor* a_this, RE::BGSLoadFormBuffer* a_buf);
	static _LoadBuffer_t* orig_LoadBuffer;


	void Hook_LoadBuffer(RE::BGSLoadFormBuffer* a_buf)
	{
		orig_LoadBuffer(this, a_buf);
		RemoveGlowFX(this);
	}


	static void InstallHooks()
	{
		if (!Settings::disableForWeapons) {
			RelocPtr<_LoadBuffer_t*> vtbl_LoadBuffer(RE::Offset::Actor::Vtbl + (0xF * 0x8));
			orig_LoadBuffer = *vtbl_LoadBuffer;
			SafeWrite64(vtbl_LoadBuffer.GetUIntPtr(), GetFnAddr(&Hook_LoadBuffer));
			_DMESSAGE("[DEBUG] Installed hooks for (%s)", typeid(ActorEx).name());
		}
	}
};


ActorEx::_LoadBuffer_t* ActorEx::orig_LoadBuffer;



class PlayerCharacterEx : public RE::PlayerCharacter
{
public:
	typedef void _LoadBuffer_t(RE::PlayerCharacter* a_this, RE::BGSLoadFormBuffer* a_buf);
	static _LoadBuffer_t* orig_LoadBuffer;


	void Hook_LoadBuffer(RE::BGSLoadFormBuffer* a_buf)
	{
		orig_LoadBuffer(this, a_buf);
		RemoveGlowFX(this);
	}


	static void InstallHooks()
	{
		if (!Settings::disableForWeapons) {
			RelocPtr<_LoadBuffer_t*> vtbl_LoadBuffer(RE::Offset::PlayerCharacter::Vtbl + (0xF * 0x8));
			orig_LoadBuffer = *vtbl_LoadBuffer;
			SafeWrite64(vtbl_LoadBuffer.GetUIntPtr(), GetFnAddr(&Hook_LoadBuffer));
			_DMESSAGE("[DEBUG] Installed hooks for (%s)", typeid(PlayerCharacterEx).name());
		}
	}
};


PlayerCharacterEx::_LoadBuffer_t* PlayerCharacterEx::orig_LoadBuffer;


void InstallHooks()
{
	ActorEx::InstallHooks();
	PlayerCharacterEx::InstallHooks();
}
