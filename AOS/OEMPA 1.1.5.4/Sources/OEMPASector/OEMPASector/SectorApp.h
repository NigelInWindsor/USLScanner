
// SectorApp.h : main header file for the SectorWeld application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Tools.h"
#include "ExternalDefinition.h"

// CSectorAppApp:
// See SectorApp.cpp for the implementation of this class
//

class CDynamicView;
class CDynamicNCView;
class CDynamicCscopeView;
class CHardwareCscopeView;
class CAscopeView;
class CBscopeView;
class CCscopeView;
class CDscopeView;
class CDscopeCorrectedView;
class CAscopeToolbar;

class CSectorAppApp : public CWinAppEx
{
public:
	CSectorAppApp();

	CMultiDocTemplate* m_pDynamicViewTemplate;			// Template for the dynamic view
	CMultiDocTemplate* m_pDynamicNCViewTemplate;		// Template for the dynamic NC view
	CMultiDocTemplate* m_pDynamicCscopeViewTemplate;	// Template for the dynamic Cscope view
	CMultiDocTemplate* m_pHardwareCscopeViewTemplate;	// Template for the dynamic Cscope view
	CMultiDocTemplate* m_pAscopeViewTemplate;			// Template for the A Scope view
	CMultiDocTemplate* m_pBscopeViewTemplate;			// Template for the B Scope view
	CMultiDocTemplate* m_pCscopeViewTemplate;			// Template for the C Scope view
	CMultiDocTemplate* m_pDscopeViewTemplate;			// Template for the D Scope view
	CMultiDocTemplate* m_pDscopeCorrectedViewTemplate;	// Template for the D Scope corrected view
	CWinThread* m_pThread;								// Thread to send the dumb Ascan data

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();

	DECLARE_MESSAGE_MAP()
};

extern CSectorAppApp theApp;

extern CDynamicView* g_pDynamicView;			// Global pointer to access the dynamic view
extern CDynamicNCView* g_pDynamicNCView;		// Global pointer to access the dynamic NC view
extern CDynamicCscopeView* g_pDynamicCscope;	// Global pointer to access the dynamic Cscope view
extern CHardwareCscopeView* g_pHardwareCscope[4+4];	// Global pointer to access the dynamic Cscope view: 4 Amplitude + 4 Time of flight.
extern CAscopeView* g_pAscope;					// Global pointer to access the A Scope view
extern CBscopeView* g_pBscope;					// Global pointer to access the B Scope view
extern CCscopeView* g_pCscope;					// Global pointer to access the C Scope view
extern CDscopeView* g_pDscope;					// Global pointer to access the D Scope view
extern CDscopeCorrectedView* g_pDscopeCorrected;// Global pointer to access the D Scope corrected view
extern CAscopeToolbar* g_pAscopeToolbar;		// Global pointer to access the Ascope toolbar