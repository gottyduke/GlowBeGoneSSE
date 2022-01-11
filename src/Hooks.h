#pragma once


namespace Hooks
{
	bool ContainerVisitor(RE::ContainerObject& a_entry);
	void RemoveGlowFX(RE::Actor* a_actor);


	class ActorEx : public RE::Actor
	{
	public:
		using func_t = decltype(&RE::Actor::LoadGame);
		static inline REL::Relocation<func_t> func;


		void Hook_LoadBuffer(RE::BGSLoadFormBuffer* a_buf)  // 0F
		{
			func(this, a_buf);
			RemoveGlowFX(this);
		}


		static void Install();
	};


	class PlayerCharacterEx : public RE::PlayerCharacter
	{
	public:
		using func_t = decltype(&RE::PlayerCharacter::LoadGame);
		static inline REL::Relocation<func_t> func;


		void Hook_LoadBuffer(RE::BGSLoadFormBuffer* a_buf)  // 0F
		{
			func(this, a_buf);
			RemoveGlowFX(this);
		}


		static void Install();
	};


	void Install();
} // namespace Hooks