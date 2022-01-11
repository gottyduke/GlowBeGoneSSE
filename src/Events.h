#pragma once

#include "Config.h"


class TaskDelegate {
	virtual void Run() = 0;
	virtual void Dispose() = 0;
};


namespace Events
{
	using EventResult = RE::BSEventNotifyControl;


	class DelayedWeaponTaskDelegate : public TaskDelegate
	{
	public:
		DelayedWeaponTaskDelegate(RE::ObjectRefHandle a_objRefHandle, std::uint32_t a_formID) :
			_objRefHandle(a_objRefHandle),
			_formID(a_formID)
		{}
		~DelayedWeaponTaskDelegate() = default;

		virtual void Run() override;
		virtual void Dispose() override { delete this; }

	private:
		RE::ObjectRefHandle	_objRefHandle;
		RE::FormID			_formID;
	};


	class DelayedActorTaskDelegate : public TaskDelegate
	{
	public:
		DelayedActorTaskDelegate(RE::ObjectRefHandle a_objRefHandle, std::uint32_t a_formID) :
			_objRefHandle(a_objRefHandle),
			_formID(a_formID)
		{}
		~DelayedActorTaskDelegate() = default;

		virtual void Run() override;
		virtual void Dispose() override { delete this; }

	private:
		RE::ObjectRefHandle	_objRefHandle;
		RE::FormID			_formID;
	};


	class TESMagicEffectApplyEventHandler : public RE::BSTEventSink<RE::TESMagicEffectApplyEvent>, public DKUtil::model::Singleton<TESMagicEffectApplyEventHandler>
	{
	public:
		virtual	EventResult ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource) override;
	};


	class TESEquipEventHandler : public RE::BSTEventSink<RE::TESEquipEvent>, public DKUtil::model::Singleton<TESMagicEffectApplyEventHandler>
	{
	public:
		virtual	EventResult	ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) override;
	};
} // namespace Events