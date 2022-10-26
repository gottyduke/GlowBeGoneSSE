#include "Hooks.h"


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
				RemoveShader(effectSetting);
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
						RemoveShader(effectSetting);
					}
				}
			}
		}
	}


	void CharacterEx::Install()
	{
		REL::Relocation<std::uintptr_t> vTbl{ VTABLE[0] };
		func = vTbl.write_vfunc(0x0F, &Hook_LoadBuffer);
		INFO("Installed hooks for Character"sv);
	}


	void PlayerCharacterEx::Install()
	{
		REL::Relocation<std::uintptr_t> vTbl{ VTABLE[0] };
		func = vTbl.write_vfunc(0x0F, &Hook_LoadBuffer);
		INFO("Installed hooks for PlayerCharacter"sv);
	}


	void Install()
	{
		//ENABLE_DEBUG
		CharacterEx::Install();
		PlayerCharacterEx::Install();
	}
} // namespace Hooks