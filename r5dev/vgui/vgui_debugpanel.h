#pragma once
#include "core/stdafx.h"
#include "mathlib/color.h"

struct CTextNotify
{
	CTextNotify(const EGlobalContext_t type, const float nTime, const string& svMessage)
	{
		this->m_svMessage       = svMessage;
		this->m_flLifeRemaining = nTime;
		this->m_type            = type;
	}
	EGlobalContext_t m_type            = EGlobalContext_t::NONE;
	float            m_flLifeRemaining = 0.0f;
	string           m_svMessage       = "";
};

class CTextOverlay
{
public:
	CTextOverlay()
	{
		m_nFontHeight = 16;
		memset(m_pszCon_NPrintf_Buf, '\0', sizeof(m_pszCon_NPrintf_Buf));
	}

	void Update(void);
	void AddLog(const EGlobalContext_t context, const string& svText);
	void DrawNotify(void);
	void DrawFormat(const int x, const int y, const Color c, const char* pszFormat, ...) const;
	void ShouldDraw(const float flFrameTime);
	void DrawHostStats(void) const;
	void DrawSimStats(void) const;
	void DrawGPUStats(void) const;
	void DrawCrosshairMaterial(void) const;
	void DrawStreamOverlay(void) const;

private:
	Color GetLogColorForType(const EGlobalContext_t type) const;
	vector<CTextNotify> m_vNotifyText;
	int m_nFontHeight; // Hardcoded to 16 in this engine.

	mutable std::mutex m_Mutex;

public:
	char m_pszCon_NPrintf_Buf[4096]{};
};

///////////////////////////////////////////////////////////////////////////////
void CEngineVGui_Attach();
void CEngineVGui_Detach();

///////////////////////////////////////////////////////////////////////////////
extern CTextOverlay* g_pOverlay;
