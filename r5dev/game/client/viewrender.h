#pragma once

//-------------------------------------------------------------------------------------
// Forward declarations
//-------------------------------------------------------------------------------------
class VMatrix;

class CViewRender
{
public:
	VMatrix* GetWorldMatrixForView(int8_t slot);
};

///////////////////////////////////////////////////////////////////////////////
const Vector3D& MainViewOrigin();
const QAngle& MainViewAngles();

inline CMemory p_CViewRender_GetWorldMatrixForView;
inline auto CViewRender_GetWorldMatrixForView = p_CViewRender_GetWorldMatrixForView.RCast<VMatrix*(*)(CViewRender*, int8_t)>();

inline Vector3D* g_vecRenderOrigin = nullptr;
inline QAngle* g_vecRenderAngles = nullptr;

inline CViewRender* g_pViewRender = nullptr;
inline CMemory g_pViewRender_VFTable;

///////////////////////////////////////////////////////////////////////////////
class V_ViewRender : public IDetour
{
	virtual void GetAdr(void) const
	{
		spdlog::debug("| FUN: CViewRender::GetWorldMatrixForView   : {:#18x} |\n", p_CViewRender_GetWorldMatrixForView.GetPtr());
		spdlog::debug("| VAR: g_vecRenderOrigin                    : {:#18x} |\n", reinterpret_cast<uintptr_t>(g_vecRenderOrigin));
		spdlog::debug("| VAR: g_vecRenderAngles                    : {:#18x} |\n", reinterpret_cast<uintptr_t>(g_vecRenderAngles));
		spdlog::debug("| VAR: g_pViewRender                        : {:#18x} |\n", reinterpret_cast<uintptr_t>(g_pViewRender));
		spdlog::debug("| CON: CViewRender                (VFTable) : {:#18x} |\n", g_pViewRender_VFTable.GetPtr());
		spdlog::debug("+----------------------------------------------------------------+\n");
	}
	virtual void GetFun(void) const
	{
		g_pViewRender_VFTable = g_GameDll.GetVirtualMethodTable(".?AVCViewRender@@");

		p_CViewRender_GetWorldMatrixForView = g_pViewRender_VFTable.WalkVTable(16).Deref(); // 16th vfunc.
		CViewRender_GetWorldMatrixForView = p_CViewRender_GetWorldMatrixForView.RCast<VMatrix* (*)(CViewRender*, int8_t)>();
	}
	virtual void GetVar(void) const
	{
		CMemory base = g_GameDll.FindPatternSIMD("48 89 74 24 ?? 57 48 83 EC 30 F3 0F 10 05 ?? ?? ?? ?? ?? 8B ??");

		g_vecRenderOrigin = base.Offset(0x00).FindPatternSelf("F3 0F 10 05").ResolveRelativeAddressSelf(0x4, 0x8).RCast<Vector3D*>();
		g_vecRenderAngles = base.Offset(0x30).FindPatternSelf("F3 0F 10 0D").ResolveRelativeAddressSelf(0x4, 0x8).RCast<QAngle*>();

		g_pViewRender = g_GameDll.FindPatternSIMD("48 8D 05 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC CC CC 48 8B C4").ResolveRelativeAddressSelf(0x3, 0x7).RCast<CViewRender*>(); /*48 8D 05 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC CC CC 48 8B C4*/
	}
	virtual void GetCon(void) const { }
	virtual void Attach(void) const { }
	virtual void Detach(void) const { }
};
///////////////////////////////////////////////////////////////////////////////

REGISTER(V_ViewRender);