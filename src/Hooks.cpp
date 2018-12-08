#include "Hooks.h"

#include "skse64_common/Relocation.h"  // RelocPtr
#include "skse64_common/SafeWrite.h"  // SafeWrite64

#include <typeinfo>  // typeid

#include "RE/EffectSetting.h"  // EffectSetting
#include "RE/TESEffectShader.h"  // TESEffectShader
#include "RE/TESDataHandler.h"  // TESDataHandler


namespace Hooks
{
	class EffectSettingEx : public RE::EffectSetting
	{
	public:
		typedef void _LoadForm_t(RE::EffectSetting* a_this, TESFile* a_mod);
		static _LoadForm_t* orig_LoadForm;


		void Hook_LoadForm(TESFile* a_mod)
		{
			orig_LoadForm(this, a_mod);

			RE::TESEffectShader* effectShader = 0;
			if (effectShader = properties.hitShader) {
				effectShader->data.fillTextureEffectPersistentAlphaRatio = 0.0;
				effectShader->data.fillTextureEffectFullAlphaRatio = 0.0;
				effectShader->data.edgeEffectPersistentAlphaRatio = 0.0;
				effectShader->data.edgeEffectFullAlphaRatio = 0.0;
			}
			if (effectShader = properties.enchantShader) {
				effectShader->data.fillTextureEffectPersistentAlphaRatio = 0.0;
				effectShader->data.fillTextureEffectFullAlphaRatio = 0.0;
				effectShader->data.edgeEffectPersistentAlphaRatio = 0.0;
				effectShader->data.edgeEffectFullAlphaRatio = 0.0;
			}
		}


		static void InstallHooks()
		{
			constexpr uintptr_t EFFECT_SETTING_VTBL = 0x0153B268;	// 1_5_62

			RelocPtr<_LoadForm_t*> vtbl_LoadForm(EFFECT_SETTING_VTBL + (0x13 * 0x8));
			orig_LoadForm = *vtbl_LoadForm;
			SafeWrite64(vtbl_LoadForm.GetUIntPtr(), GetFnAddr(&Hook_LoadForm));

			_DMESSAGE("[DEBUG] Installed hooks for (%s)", typeid(EffectSettingEx).name());
		}
	};


	EffectSettingEx::_LoadForm_t* EffectSettingEx::orig_LoadForm;


	void InstallHooks()
	{
		EffectSettingEx::InstallHooks();
	}
}
