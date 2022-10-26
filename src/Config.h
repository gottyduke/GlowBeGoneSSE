#pragma once

namespace Config
{
	using namespace DKUtil::Alias;

	extern Boolean RemoveActorFX;
	extern Boolean RemoveWeaponFX;
	extern String ExclusionList;
	extern std::unordered_set<RE::FormID> ExclusionID;

	void Load();
	bool IsExcluded(const RE::FormID a_formID);
}  // namespace Config

namespace
{
	inline void RemoveShader(RE::EffectSetting* a_effect) noexcept
	{
		RE::TESEffectShader* effectShader = a_effect->data.effectShader;

		if (effectShader) {
			effectShader->data.fillTextureEffectPersistentAlphaRatio = 0.f;
			effectShader->data.fillTextureEffectFullAlphaRatio = 0.f;
			effectShader->data.edgeEffectPersistentAlphaRatio = 0.f;
			effectShader->data.edgeEffectFullAlphaRatio = 0.f;
		}

		if (effectShader = a_effect->data.enchantShader) {
			effectShader->data.fillTextureEffectPersistentAlphaRatio = 0.f;
			effectShader->data.fillTextureEffectFullAlphaRatio = 0.f;
			effectShader->data.edgeEffectPersistentAlphaRatio = 0.f;
			effectShader->data.edgeEffectFullAlphaRatio = 0.f;
		}
	}
}