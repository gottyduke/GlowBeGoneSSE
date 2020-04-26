#include "skse64/gamethreads.h"  // TaskDelegate

#include "RE/Skyrim.h"


class DelayedWeaponTaskDelegate : public TaskDelegate
{
public:
<<<<<<< Updated upstream
	DelayedWeaponTaskDelegate(RE::RefHandle a_refHandle, UInt32 a_formID);
=======
	DelayedWeaponTaskDelegate(RE::ObjectRefHandle a_objRefHandle, UInt32 a_formID);
>>>>>>> Stashed changes
	~DelayedWeaponTaskDelegate() = default;

	void Run() override;
	void Dispose() override;

private:
<<<<<<< Updated upstream
	RE::RefHandle	_refHandle;
	UInt32			_formID;
=======
	RE::ObjectRefHandle _objRefHandle;
	RE::FormID _formID;
>>>>>>> Stashed changes
};


class DelayedActorTaskDelegate : public TaskDelegate
{
public:
<<<<<<< Updated upstream
	DelayedActorTaskDelegate(RE::RefHandle a_refHandle, UInt32 a_formID);
=======
	DelayedActorTaskDelegate(RE::ObjectRefHandle a_objRefHandle, UInt32 a_formID);
>>>>>>> Stashed changes
	~DelayedActorTaskDelegate() = default;

	void Run() override;
	void Dispose() override;

private:
<<<<<<< Updated upstream
	RE::RefHandle	_refHandle;
	UInt32	_formID;
=======
	RE::ObjectRefHandle _objRefHandle;
	RE::FormID _formID;
>>>>>>> Stashed changes
};


class TESMagicEffectApplyEventHandler final : public RE::BSTEventSink<RE::TESMagicEffectApplyEvent>
{
public:
	using EventResult = RE::EventResult;

<<<<<<< Updated upstream

	static TESMagicEffectApplyEventHandler* GetSingleton();
	virtual	RE::EventResult	ReceiveEvent(RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource) override;

private:
	TESMagicEffectApplyEventHandler() = default;
=======
	static TESMagicEffectApplyEventHandler* GetSingleton();
	EventResult ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource) override;

>>>>>>> Stashed changes
	TESMagicEffectApplyEventHandler(const TESMagicEffectApplyEventHandler&) = delete;
	TESMagicEffectApplyEventHandler(TESMagicEffectApplyEventHandler&&) = delete;
	TESMagicEffectApplyEventHandler& operator=(const TESMagicEffectApplyEventHandler&) = delete;
	TESMagicEffectApplyEventHandler& operator=(TESMagicEffectApplyEventHandler&&) = delete;

private:
	TESMagicEffectApplyEventHandler() = default;
	virtual ~TESMagicEffectApplyEventHandler() = default;
};


class TESEquipEventHandler final : public RE::BSTEventSink<RE::TESEquipEvent>
{
public:
	using EventResult = RE::EventResult;

	static TESEquipEventHandler* GetSingleton();
<<<<<<< Updated upstream
	virtual	RE::EventResult	ReceiveEvent(RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) override;

private:
	TESEquipEventHandler() = default;
=======
	EventResult ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) override;
	
>>>>>>> Stashed changes
	TESEquipEventHandler(const TESEquipEventHandler&) = delete;
	TESEquipEventHandler(TESEquipEventHandler&&) = delete;
	TESEquipEventHandler& operator=(const TESEquipEventHandler&) = delete;
	TESEquipEventHandler& operator=(TESEquipEventHandler&&) = delete;

private:
	TESEquipEventHandler() = default;
	virtual ~TESEquipEventHandler() = default;
};
