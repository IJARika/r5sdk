#pragma once
#include <engine/server/sv_main.h>
#include <vguimatsurface/MatSystemSurface.h>

enum class PaintMode_t
{
	PAINT_UIPANELS     = (1 << 0),
	PAINT_INGAMEPANELS = (1 << 1),
};

class CEngineVGui
{
public:
	static int Paint(CEngineVGui* thisptr, PaintMode_t mode);
	void EnabledProgressBarForNextLoad(void)
	{
		int index = 31;
		CallVFunc<void>(index, this);
	}
	void ShowErrorMessage(void)
	{
		int index = 35;
		CallVFunc<void>(index, this);
	}
	void HideLoadingPlaque(void)
	{
		int index = 36;
		CallVFunc<void>(index, this);
	}
};

/* ==== CENGINEVGUI ===================================================================================================================================================== */
inline CMemory p_CEngineVGui_Paint;
inline auto CEngineVGui_Paint = p_CEngineVGui_Paint.RCast<int (*)(CEngineVGui* thisptr, PaintMode_t mode)>();

inline CMemory p_CEngineVGui_RenderStart;
inline auto CEngineVGui_RenderStart = p_CEngineVGui_RenderStart.RCast<void* (*)(CMatSystemSurface* pMatSystemSurface)>();

inline CMemory p_CEngineVGui_RenderEnd;
inline auto CEngineVGui_RenderEnd = p_CEngineVGui_RenderEnd.RCast<void* (*)(void)>();

inline CEngineVGui* g_pEngineVGui = nullptr;

///////////////////////////////////////////////////////////////////////////////
class VEngineVGui : public IDetour
{
	virtual void GetAdr(void) const
	{
		spdlog::debug("| FUN: CEngineVGui::Paint                   : {:#18x} |\n", p_CEngineVGui_Paint.GetPtr());
		spdlog::debug("| FUN: CEngineVGui::RenderStart             : {:#18x} |\n", p_CEngineVGui_RenderStart.GetPtr());
		spdlog::debug("| FUN: CEngineVGui::RenderEnd               : {:#18x} |\n", p_CEngineVGui_RenderEnd.GetPtr());
		spdlog::debug("| VAR: g_pEngineVGui                        : {:#18x} |\n", reinterpret_cast<uintptr_t>(g_pEngineVGui));
		spdlog::debug("+----------------------------------------------------------------+\n");
	}
	virtual void GetFun(void) const
	{
#if defined (GAMEDLL_S0) || defined (GAMEDLL_S1)
		p_CEngineVGui_Paint = g_GameDll.FindPatternSIMD("89 54 24 10 55 56 41 55 48 81 EC ?? ?? ?? ??");
		CEngineVGui_Paint = p_CEngineVGui_Paint.RCast<int (*)(CEngineVGui* thisptr, PaintMode_t mode)>(); /*41 55 41 56 48 83 EC 78 44 8B EA*/

		p_CEngineVGui_RenderStart = g_GameDll.FindPatternSIMD("48 8B C4 53 56 57 48 81 EC ?? ?? ?? ?? 0F 29 70 D8");
		CEngineVGui_RenderStart = p_CEngineVGui_RenderStart.RCast<void* (*)(CMatSystemSurface*)>(); /*48 8B C4 53 56 57 48 81 EC ?? ?? ?? ?? 0F 29 70 D8*/
#elif defined (GAMEDLL_S2) || defined (GAMEDLL_S3)
		p_CEngineVGui_Paint = g_GameDll.FindPatternSIMD("41 55 41 56 48 83 EC 78 44 8B EA");
		CEngineVGui_Paint = p_CEngineVGui_Paint.RCast<int (*)(CEngineVGui* thisptr, PaintMode_t mode)>(); /*41 55 41 56 48 83 EC 78 44 8B EA*/

		p_CEngineVGui_RenderStart = g_GameDll.FindPatternSIMD("40 53 57 48 81 EC ?? ?? ?? ?? 48 8B F9");
		CEngineVGui_RenderStart = p_CEngineVGui_RenderStart.RCast<void* (*)(CMatSystemSurface*)>(); /*40 53 57 48 81 EC ?? ?? ?? ?? 48 8B F9*/
#endif
		p_CEngineVGui_RenderEnd = g_GameDll.FindPatternSIMD("40 53 48 83 EC 20 48 8B 0D ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ?? 48 8B 01");
		CEngineVGui_RenderEnd = p_CEngineVGui_RenderEnd.RCast<void* (*)(void)>(); /*40 53 48 83 EC 20 48 8B 0D ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ?? 48 8B 01*/
	}
	virtual void GetVar(void) const
	{
		g_pEngineVGui = g_GameDll.FindPatternSIMD("48 8B C4 48 89 48 08 48 89 50 10 4C 89 40 18 4C 89 48 20 53 57 48 81 EC ?? ?? ?? ?? 48 8B D9 48 8D 78 10 E8 ?? ?? ?? ?? 48 89 7C 24 ?? 48 8D 54 24 ?? 33 FF 4C 8B CB 41 B8 ?? ?? ?? ?? 48 89 7C 24 ?? 48 8B 08 48 83 C9 01 E8 ?? ?? ?? ?? 85 C0 48 8D 54 24 ??")
			.FindPatternSelf("48 8D ?? ?? ?? ?? 01", CMemory::Direction::DOWN, 150).ResolveRelativeAddressSelf(0x3, 0x7).RCast<CEngineVGui*>();
	}
	virtual void GetCon(void) const { }
	virtual void Attach(void) const { }
	virtual void Detach(void) const { }
};
///////////////////////////////////////////////////////////////////////////////

REGISTER(VEngineVGui);