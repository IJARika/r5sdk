#pragma once

/* ==== STRYDER ================================================================================================================================================ */
inline CMemory p_Stryder_StitchRequest;
inline auto Stryder_StitchRequest = p_Stryder_StitchRequest.RCast<void*(*)(void* a1)>();

inline CMemory p_Stryder_SendOfflineRequest;
inline auto Stryder_SendOfflineRequest = p_Stryder_SendOfflineRequest.RCast<bool(*)(void)>();

///////////////////////////////////////////////////////////////////////////////
class VStryder : public IDetour
{
	virtual void GetAdr(void) const
	{
		spdlog::debug("| FUN: Stryder_StitchRequest                : {:#18x} |\n", p_Stryder_StitchRequest.GetPtr());
		spdlog::debug("| FUN: Stryder_SendOfflineRequest           : {:#18x} |\n", p_Stryder_SendOfflineRequest.GetPtr());
		spdlog::debug("+----------------------------------------------------------------+\n");
	}
	virtual void GetFun(void) const
	{
#if defined (GAMEDLL_S0) || defined (GAMEDLL_S1)
		p_Stryder_StitchRequest      = g_GameDll.FindPatternSIMD("48 8B C4 53 57 41 56 48 81 EC 20");
		p_Stryder_SendOfflineRequest = g_GameDll.FindPatternSIMD("48 89 5C 24 ?? 48 89 74 24 ?? 48 89 7C 24 ?? 55 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B 35 ?? ?? ?? ?? 48 8D 05 ?? ?? ?? ?? 83 65 D0 FC 48 8D 4D 80");
#elif defined (GAMEDLL_S2) || defined (GAMEDLL_S3)
		p_Stryder_StitchRequest      = g_GameDll.FindPatternSIMD("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 48 8B F9 E8 B4");
		p_Stryder_SendOfflineRequest = g_GameDll.FindPatternSIMD("48 89 5C 24 ?? 48 89 74 24 ?? 55 57 41 56 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B 35 ?? ?? ?? ??");
#endif
		Stryder_StitchRequest      = p_Stryder_StitchRequest.RCast<void* (*)(void*)>();   /*48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 48 8B F9 E8 B4*/
		Stryder_SendOfflineRequest = p_Stryder_SendOfflineRequest.RCast<bool(*)(void)>(); /*48 89 5C 24 ?? 48 89 74 24 ?? 55 57 41 56 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B 35 ?? ?? ?? ??*/
	}
	virtual void GetVar(void) const { }
	virtual void GetCon(void) const { }
	virtual void Attach(void) const { }
	virtual void Detach(void) const { }
};
///////////////////////////////////////////////////////////////////////////////

REGISTER(VStryder);