#include "Hooks.h"
#include "Config.h"


namespace Hooks 
{
	bool ContainerVisitor(RE::ContainerObject& a_entry)
	{
		if (!a_entry.obj || a_entry.obj->IsNot(RE::FormType::Weapon)) {
			return true;
		}

		auto* weap = static_cast<RE::TESObjectWEAP*>(a_entry.obj);
		auto* ench = weap->formEnchanting;
		if (!ench) {
			return true;
		}

		for (auto* effect : ench->effects) {
			if (!effect) {
				continue;
			}
			
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
		return true;
	}


	void RemoveGlowFX(RE::Actor* a_actor)
	{
		auto* container = a_actor->GetContainer();
		if (container) {
			container->ForEachContainerObject(ContainerVisitor);
		}

		auto* changes = a_actor->GetInventoryChanges();
		if (!changes || !changes->entryList) {
			return;
		}

		for (auto* entry : *changes->entryList) {
			if (!entry->object || !entry->extraLists) {
				continue;
			}
			for (auto* xList : *entry->extraLists) {
				if (!xList || !xList->HasType(RE::ExtraDataType::kWorn)) {
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

				for (auto* effect : ench->effects) {
					if (!effect) {
						continue;
					}
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


	void ActorEx::Install() {
		if (*Config::RemoveActorFX) {
			REL::Relocation<std::uintptr_t> vTbl{ RE::Offset::Actor::Vtbl };
			func = vTbl.write_vfunc(0x0F, &RE::Actor::LoadGame);
			INFO("Installed hooks for Actor"sv);
		}
	}


	void PlayerCharacterEx::Install() {
		if (*Config::RemoveActorFX) {
			REL::Relocation<std::uintptr_t> vTbl{ RE::Offset::Actor::Vtbl };
			func = vTbl.write_vfunc(0x0F, &RE::PlayerCharacter::LoadGame);
			INFO("Installed hooks for PlayerCharacter"sv);
		}
	}


	void Install()
	{
		ActorEx::Install();
		PlayerCharacterEx::Install();
	}
} // namespace Hooks