#include "skse64/gamethreads.h"  // TaskDelegate

#include "RE/BSTEvent.h"  // BSTEventSink, EventResult, BSTEventSource
#include "RE/FormTypes.h"  // Actor
#include "RE/Memory.h"  // TES_HEAP_REDEFINE_NEW
#include "RE/TESMagicEffectApplyEvent.h"  // TESMagicEffectApplyEvent

struct SKSETaskInterface;


class DelayedTaskDelegate : public TaskDelegate
{
public:
	constexpr DelayedTaskDelegate(RE::Actor* a_actor, UInt32 a_formID) :
		_actor(a_actor),
		_formID(a_formID)
	{}

	virtual void Run() override;
	virtual void Dispose() override;

	TES_HEAP_REDEFINE_NEW();

private:
	RE::Actor*	_actor;
	UInt32		_formID;
};


class TESMagicEffectApplyEventHandler : public RE::BSTEventSink<RE::TESMagicEffectApplyEvent>
{
public:
	virtual ~TESMagicEffectApplyEventHandler();
	virtual	RE::EventResult	ReceiveEvent(RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource) override;
};


extern SKSETaskInterface* g_task;
extern TESMagicEffectApplyEventHandler g_magicEffectApplyEventHandler;
