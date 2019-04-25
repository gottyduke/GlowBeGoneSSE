#include "skse64/gamethreads.h"  // TaskDelegate

#include "RE/Skyrim.h"


class DelayedWeaponTaskDelegate : public TaskDelegate
{
public:
	DelayedWeaponTaskDelegate(RE::RefHandle a_refHandle, UInt32 a_formID);
	~DelayedWeaponTaskDelegate() = default;

	virtual void Run() override;
	virtual void Dispose() override;

private:
	RE::RefHandle	_refHandle;
	UInt32			_formID;
};


class DelayedActorTaskDelegate : public TaskDelegate
{
public:
	DelayedActorTaskDelegate(RE::RefHandle a_refHandle, UInt32 a_formID);
	~DelayedActorTaskDelegate() = default;

	virtual void Run() override;
	virtual void Dispose() override;

private:
	RE::RefHandle	_refHandle;
	UInt32	_formID;
};


class TESMagicEffectApplyEventHandler : public RE::BSTEventSink<RE::TESMagicEffectApplyEvent>
{
public:
	using EventResult = RE::EventResult;


	static TESMagicEffectApplyEventHandler* GetSingleton();
	virtual	RE::EventResult	ReceiveEvent(RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource) override;

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
	using EventResult = RE::EventResult;


	static TESEquipEventHandler* GetSingleton();
	virtual	RE::EventResult	ReceiveEvent(RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) override;

private:
	TESEquipEventHandler() = default;
	TESEquipEventHandler(const TESEquipEventHandler&) = delete;
	TESEquipEventHandler(TESEquipEventHandler&&) = delete;
	virtual ~TESEquipEventHandler() = default;

	TESEquipEventHandler& operator=(const TESEquipEventHandler&) = delete;
	TESEquipEventHandler& operator=(TESEquipEventHandler&&) = delete;
};
