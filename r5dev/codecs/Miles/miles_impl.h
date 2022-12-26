#pragma once
#include "miles_types.h"

/* ==== WASAPI THREAD SERVICE =========================================================================================================================================== */
inline CMemory p_AIL_LogFunc;
inline auto v_AIL_LogFunc = p_AIL_LogFunc.RCast<void(*)(int64_t nLogLevel, const char* pszMessage)>();

inline CMemory p_Miles_Initialize;
inline auto v_Miles_Initialize = p_Miles_Initialize.RCast<bool(*)()>();

inline CMemory p_MilesQueueEventRun;
inline auto v_MilesQueueEventRun = p_MilesQueueEventRun.RCast<void(*)(Miles::Queue*, const char*)>();

inline CMemory p_MilesBankPatch;
inline auto v_MilesBankPatch = p_MilesBankPatch.RCast<void(*)(Miles::Bank*, char*, char*)>();

void MilesCore_Attach();
void MilesCore_Detach();

///////////////////////////////////////////////////////////////////////////////
class MilesCore : public IDetour
{
	virtual void GetAdr(void) const
	{
		spdlog::debug("| FUN: AIL_LogFunc                          : {:#18x} |\n", p_AIL_LogFunc.GetPtr());
		spdlog::debug("| FUN: Miles_Initialize                     : {:#18x} |\n", p_Miles_Initialize.GetPtr());
		spdlog::debug("+----------------------------------------------------------------+\n");
	}
	virtual void GetFun(void) const
	{
		p_AIL_LogFunc = g_GameDll.FindPatternSIMD("40 53 48 83 EC 20 48 8B DA 48 8D 15 ?? ?? ?? ??");
		v_AIL_LogFunc = p_AIL_LogFunc.RCast<void(*)(int64_t, const char*)>();
		// 0x1409D1420 // 40 53 48 83 EC 20 48 8B DA 48 8D 15 ? ? ? ? //

		p_Miles_Initialize = g_GameDll.FindPatternSIMD("E8 ?? ?? ?? ?? FF 0D ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ??").FollowNearCallSelf();
		v_Miles_Initialize = p_Miles_Initialize.RCast<bool(*)()>();
		// 0x14095A140 // E8 ? ? ? ? FF 0D ? ? ? ? C6 05 ? ? ? ? ?  //

		p_MilesQueueEventRun = g_RadAudioSystemDll.GetExportedFunction("MilesQueueEventRun");
		v_MilesQueueEventRun = p_MilesQueueEventRun.RCast<void(*)(Miles::Queue*, const char*)>();

		p_MilesBankPatch = g_RadAudioSystemDll.GetExportedFunction("MilesBankPatch");
		v_MilesBankPatch = p_MilesBankPatch.RCast<void(*)(Miles::Bank*, char*, char*)>();

	}
	virtual void GetVar(void) const { }
	virtual void GetCon(void) const { }
	virtual void Attach(void) const { }
	virtual void Detach(void) const { }
};
///////////////////////////////////////////////////////////////////////////////

REGISTER(MilesCore);
