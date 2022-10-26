#pragma once

#include "Config.h"

namespace Hooks
{
	bool ContainerVisitor(RE::ContainerObject& a_entry);
	void RemoveGlowFX(RE::Actor* a_actor);

	class CharacterEx : public RE::Character
	{
	public:
		static inline REL::Relocation<decltype(&RE::Character::LoadGame)> func;

		void Hook_LoadBuffer(RE::BGSLoadFormBuffer* a_buf)  // 0F
		{
			DEBUG("Load Character: {}|{:X}", GetName(), GetFormID());
			func(this, a_buf);
			RemoveGlowFX(this);
		}

		static void Install();
	};

	class PlayerCharacterEx : public RE::PlayerCharacter
	{
	public:
		static inline REL::Relocation<decltype(&RE::PlayerCharacter::LoadGame)> func;

		void Hook_LoadBuffer(RE::BGSLoadFormBuffer* a_buf)  // 0F
		{
			DEBUG("Load PC: {}|{:X}", GetName(), GetFormID());
			func(this, a_buf);
			RemoveGlowFX(this);
		}

		static void Install();
	};

	void Install();
}  // namespace Hooks