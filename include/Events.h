#pragma once

#include "skse64/gamethreads.h"  // TaskDelegate

#include "Settings.h"

#include "RE/Skyrim.h"


class DelayedWeaponTaskDelegate : public TaskDelegate
{
public:
	DelayedWeaponTaskDelegate(RE::ObjectRefHandle a_refHandle, UInt32 a_formID);
	~DelayedWeaponTaskDelegate() = default;

	virtual void Run() override;
	virtual void Dispose() override;

private:
	RE::ObjectRefHandle	_objRefHandle;
	RE::FormID			_formID;
};


class DelayedActorTaskDelegate : public TaskDelegate
{
public:
	DelayedActorTaskDelegate(RE::ObjectRefHandle a_refHandle, UInt32 a_formID);
	~DelayedActorTaskDelegate() = default;

	virtual void Run() override;
	virtual void Dispose() override;

private:
	RE::ObjectRefHandle	_objRefHandle;
	RE::FormID			_formID;
};


class TESMagicEffectApplyEventHandler : public RE::BSTEventSink<RE::TESMagicEffectApplyEvent>
{
public:
	using EventResult = RE::BSEventNotifyControl;

	static TESMagicEffectApplyEventHandler* GetSingleton();

	virtual	EventResult ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource) override;

private:
	TESMagicEffectApplyEventHandler() = default;
	TESMagicEffectApplyEventHandler(const TESMagicEffectApplyEventHandler&) = delete;
	TESMagicEffectApplyEventHandler(TESMagicEffectApplyEventHandler&&) = delete;
	virtual ~TESMagicEffectApplyEventHandler() = default;

	TESMagicEffectApplyEventHandler& operator=(const TESMagicEffectApplyEventHandler&) = delete;
	TESMagicEffectApplyEventHandler& operator=(TESMagicEffectApplyEventHandler&&) = delete;
};


class TESEquipEventHandler : public RE::BSTEventSink<RE::TESEquipEvent>
{
public:
	using EventResult = RE::BSEventNotifyControl;


	static TESEquipEventHandler* GetSingleton();
	virtual	EventResult	ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) override;

private:
	TESEquipEventHandler() = default;
	TESEquipEventHandler(const TESEquipEventHandler&) = delete;
	TESEquipEventHandler(TESEquipEventHandler&&) = delete;
	virtual ~TESEquipEventHandler() = default;

	TESEquipEventHandler& operator=(const TESEquipEventHandler&) = delete;
	TESEquipEventHandler& operator=(TESEquipEventHandler&&) = delete;
};
