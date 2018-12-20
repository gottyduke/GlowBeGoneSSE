#include "skse64/gamethreads.h"  // TaskDelegate

#include "RE/BSAnimationGraphEvent.h"  // BSAnimationGraphEvent
#include "RE/BSTEvent.h"  // BSTEventSink, EventResult, BSTEventSource
#include "RE/FormTypes.h"  // TESObjectWEAP, Actor
#include "RE/Memory.h"  // TES_HEAP_REDEFINE_NEW
#include "RE/TESEquipEvent.h"  // TESEquipEvent
#include "RE/TESMagicEffectApplyEvent.h"  // TESMagicEffectApplyEvent

struct SKSETaskInterface;


class DelayedWeaponTaskDelegate : public TaskDelegate
{
public:
	constexpr DelayedWeaponTaskDelegate(UInt32 a_refHandle, UInt32 a_formID) :
		_refHandle(a_refHandle),
		_formID(a_formID)
	{}

	virtual void Run() override;
	virtual void Dispose() override;

	TES_HEAP_REDEFINE_NEW();

private:
	UInt32	_refHandle;
	UInt32	_formID;
};


class DelayedActorTaskDelegate : public TaskDelegate
{
public:
	constexpr DelayedActorTaskDelegate(UInt32 a_refHandle, UInt32 a_formID) :
		_refHandle(a_refHandle),
		_formID(a_formID)
	{}

	virtual void Run() override;
	virtual void Dispose() override;

	TES_HEAP_REDEFINE_NEW();

private:
	UInt32	_refHandle;
	UInt32	_formID;
};


class TESMagicEffectApplyEventHandler : public RE::BSTEventSink<RE::TESMagicEffectApplyEvent>
{
public:
	virtual ~TESMagicEffectApplyEventHandler();
	virtual	RE::EventResult	ReceiveEvent(RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource) override;
};


class TESEquipEventHandler : public RE::BSTEventSink<RE::TESEquipEvent>
{
public:
	virtual ~TESEquipEventHandler();
	virtual	RE::EventResult	ReceiveEvent(RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) override;
};


extern SKSETaskInterface* g_task;
extern TESMagicEffectApplyEventHandler g_magicEffectApplyEventHandler;
extern TESEquipEventHandler g_equipEventHandler;
