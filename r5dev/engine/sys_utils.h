#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline CMemory p_Sys_Error;
inline auto v_Sys_Error = p_Sys_Error.RCast<void (*)(char* fmt, ...)>();

inline CMemory p_Sys_Warning;
inline auto v_Sys_Warning = p_Sys_Warning.RCast<void* (*)(int, char* fmt, ...)>(); 

inline CMemory p_Sys_GetProcessUpTime;
inline auto v_Sys_GetProcessUpTime = p_Sys_GetProcessUpTime.RCast<int (*)(char* szBuffer)>();
#ifndef DEDICATED
inline CMemory p_Con_NPrintf;
inline auto v_Con_NPrintf = p_Con_NPrintf.RCast<void (*)(int pos, const char* fmt, ...)>();
#endif // !DEDICATED
/* ==== ------- ========================================================================================================================================================= */

///////////////////////////////////////////////////////////////////////////////
void HSys_Error(char* fmt, ...);
int Sys_GetProcessUpTime(char* szBuffer);

void SysUtils_Attach();
void SysUtils_Detach();

///////////////////////////////////////////////////////////////////////////////
class VSys_Utils : public IDetour
{
	virtual void GetAdr(void) const
	{
		spdlog::debug("| FUN: Sys_Error                            : {:#18x} |\n", p_Sys_Error.GetPtr());
		spdlog::debug("| FUN: Sys_Warning                          : {:#18x} |\n", p_Sys_Warning.GetPtr());
		spdlog::debug("| FUN: Sys_GetProcessUpTime                 : {:#18x} |\n", p_Sys_GetProcessUpTime.GetPtr());
#ifndef DEDICATED
		spdlog::debug("| FUN: Con_NPrintf                          : {:#18x} |\n", p_Con_NPrintf.GetPtr());
#endif // !DEDICATED
		spdlog::debug("+----------------------------------------------------------------+\n");
	}
	virtual void GetFun(void) const
	{
		p_Sys_Error            = g_GameDll.FindPatternSIMD("48 89 4C 24 08 48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 53 55 41 54 41 56 B8 58 10 ?? ?? E8");
		p_Sys_Warning          = g_GameDll.FindPatternSIMD("48 89 54 24 ?? 4C 89 44 24 ?? 4C 89 4C 24 ?? 48 83 EC 28 4C 8D 44 24 ?? E8 ?? ?? ?? ?? 48 83 C4 28 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 89 5C 24 ?? 48 89 74 24 ?? 48 89 7C 24 ?? 8B 05 ?? ?? ?? ??");
		p_Sys_GetProcessUpTime = g_GameDll.FindPatternSIMD("40 57 48 83 EC 30 48 8B F9 8B 0D ?? ?? ?? ??");
#ifndef DEDICATED
		p_Con_NPrintf          = g_GameDll.FindPatternSIMD("48 89 4C 24 ?? 48 89 54 24 ?? 4C 89 44 24 ?? 4C 89 4C 24 ?? C3");
#endif // !DEDICATED
		v_Sys_Error            = p_Sys_Error.RCast<void (*)(char*, ...)>();                                     /*48 89 4C 24 08 48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 53 55 41 54 41 56 B8 58 10 00 00 E8*/
		v_Sys_Warning          = p_Sys_Warning.RCast<void* (*)(int, char*, ...)>();                             /*48 89 54 24 ?? 4C 89 44 24 ?? 4C 89 4C 24 ?? 48 83 EC 28 4C 8D 44 24 ?? E8 ?? ?? ?? ?? 48 83 C4 28 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 89 5C 24 ?? 48 89 74 24 ?? 48 89 7C 24 ?? 8B 05 ?? ?? ?? ??*/
		v_Sys_GetProcessUpTime = p_Sys_GetProcessUpTime.RCast<int (*)(char*)>();                                /*40 57 48 83 EC 30 48 8B F9 8B 0D ?? ?? ?? ??*/
#ifndef DEDICATED
		v_Con_NPrintf          = p_Con_NPrintf.RCast<void (*)(int, const char*, ...)>();                        /*48 89 4C 24 ?? 48 89 54 24 ?? 4C 89 44 24 ?? 4C 89 4C 24 ?? C3*/
#endif // !DEDICATED
	}
	virtual void GetVar(void) const { }
	virtual void GetCon(void) const { }
	virtual void Attach(void) const { }
	virtual void Detach(void) const { }
};
///////////////////////////////////////////////////////////////////////////////

REGISTER(VSys_Utils);
