// AscopeToolbar.cpp : implementation file
//

#include "stdafx.h"
#include "SectorApp.h"
#include "AscopeToolbar.h"
#include "AscopeView.h"
#include "DynamicView.h"

// CAscopeToolbar dialog

IMPLEMENT_DYNAMIC(CAscopeToolbar, CMyDialogBar)

CAscopeToolbar::CAscopeToolbar(CWnd* pParent /*=NULL*/) : CMyDialogBar()
	, m_iProbe(0)
	, m_iCycle(0)
	, m_iDisplayMode(0)
{
	g_pAscopeToolbar = this;
}

CAscopeToolbar::~CAscopeToolbar()
{
	g_pAscopeToolbar = NULL;
}

void CAscopeToolbar::DoDataExchange(CDataExchange* pDX)
{
	CMyDialogBar::DoDataExchange(pDX);
	
	DDX_CBIndex(pDX, IDC_COMBO_PROBE, m_iProbe);
	DDX_CBIndex(pDX, IDC_COMBO_CYCLE, m_iCycle);
	DDX_CBIndex(pDX, IDC_COMBO_DISPLAY_MODE, m_iDisplayMode);
	DDX_Control(pDX, IDC_COMBO_CYCLE, m_cCycle);
}

BEGIN_MESSAGE_MAP(CAscopeToolbar, CMyDialogBar)
	ON_CBN_SELCHANGE(IDC_COMBO_PROBE, &CAscopeToolbar::OnCbnSelchangeComboProbe)
	ON_CBN_SELCHANGE(IDC_COMBO_CYCLE, &CAscopeToolbar::OnCbnSelchangeComboCycle)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY_MODE, &CAscopeToolbar::OnCbnSelchangeComboDisplayMode)
END_MESSAGE_MAP()

// CAscopeToolbar message handlers

BOOL CAscopeToolbar::OnInitDialogBar()
{
	CMyDialogBar::OnInitDialogBar();
	FillCycleCombo();
	OnCbnSelchangeComboCycle();
	return TRUE;
}

void CAscopeToolbar::FillCycleCombo()
{
	m_cCycle.ResetContent();
	CString csText(_T(""));

	int CycleQuantity = 0;

	if (g_uiProbe == 0)
		CycleQuantity = InfoProbe1.iCycleCount;
	else if (g_uiProbe == 1)
		CycleQuantity = InfoProbe2.iCycleCount;
	
	for (int i = 0 ; i < CycleQuantity ; i++)
	{
		if (i < 9)
			csText.Format(_T("Cycle 0%d"), i + 1);
		else
			csText.Format(_T("Cycle %d"), i + 1);

		m_cCycle.AddString(csText);
	}

	m_iCycle = 0;

	UpdateData(FALSE);
}

void CAscopeToolbar::OnCbnSelchangeComboProbe()
{
	UpdateData();
	g_pAscope->m_iProbe = m_iProbe;

	FillCycleCombo();
}

void CAscopeToolbar::OnCbnSelchangeComboCycle()
{
	UpdateData();
	g_pAscope->m_iCycle = m_iCycle;
	if(g_pDynamicView)
		g_pDynamicView->SetCycle(m_iCycle);
}

void CAscopeToolbar::OnCbnSelchangeComboDisplayMode()
{
	UpdateData();
	g_pAscope->m_iDisplayMode = m_iDisplayMode;
}