// PositionHoldingFokkerPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PositionHoldingFokkerPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPositionHoldingFokkerPage property page

IMPLEMENT_DYNCREATE(CPositionHoldingFokkerPage, CResizablePage)

CPositionHoldingFokkerPage::CPositionHoldingFokkerPage() : CResizablePage(CPositionHoldingFokkerPage::IDD)
{
	//{{AFX_DATA_INIT(CPositionHoldingFokkerPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pFont=NULL;
	

}

CPositionHoldingFokkerPage::~CPositionHoldingFokkerPage()
{
	delete m_pFont;
}

void CPositionHoldingFokkerPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPositionHoldingFokkerPage)
	DDX_Control(pDX, IDC_EDIT_X_FIXED, m_editXFixed);
	DDX_Control(pDX, IDC_STATIC_XFIXED, m_staticXFixed);
	DDX_Control(pDX, IDC_STATIC_MOVING, m_staticMoving);
	DDX_Control(pDX, IDC_STATIC_FIXED, m_staticFixed);
	DDX_Control(pDX, IDC_STATIC_XMOVING, m_staticXMoving);
	DDX_Control(pDX, IDC_STATIC_ZMOVING, m_staticZMoving);
	DDX_Control(pDX, IDC_STATIC_ZFIXED, m_staticZFixed);
	DDX_Control(pDX, IDC_STATIC_YMOVING, m_staticYMoving);
	DDX_Control(pDX, IDC_STATIC_YFIXED, m_staticYFixed);
	DDX_Control(pDX, IDC_EDIT_Z_MOVING, m_editZMoving);
	DDX_Control(pDX, IDC_EDIT_Z_FIXED, m_editZFixed);
	DDX_Control(pDX, IDC_EDIT_Y_MOVING, m_editYMoving);
	DDX_Control(pDX, IDC_EDIT_Y_FIXED, m_editYFixed);
	DDX_Control(pDX, IDC_EDIT_X_MOVING, m_editXMoving);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPositionHoldingFokkerPage, CResizablePage)
	//{{AFX_MSG_MAP(CPositionHoldingFokkerPage)
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPositionHoldingFokkerPage message handlers

BOOL CPositionHoldingFokkerPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	
	GetClientRect(m_rrClient);

	AddAnchor(&m_editXMoving);
	AddAnchor(&m_editYMoving);
	AddAnchor(&m_editZMoving);
	AddAnchor(&m_editXFixed);
	AddAnchor(&m_editYFixed);
	AddAnchor(&m_editZFixed);
	AddAnchor(&m_staticXMoving);
	AddAnchor(&m_staticYMoving);
	AddAnchor(&m_staticZMoving);
	AddAnchor(&m_staticXFixed);
	AddAnchor(&m_staticYFixed);
	AddAnchor(&m_staticZFixed);
	AddAnchor(&m_staticFixed);
	AddAnchor(&m_staticMoving);

	CFont* pFont = GetFont();
	pFont->GetLogFont(&m_LogFont);

	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&m_LogFont);
	

	SetTimer(1,500,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPositionHoldingFokkerPage::AddAnchor(CWnd *pCtrl)
{
	CRect rr;

	pCtrl->GetWindowRect(&rr);
	ScreenToClient(&rr);

	int nID = pCtrl->GetDlgCtrlID();
	CResizablePage::AddAnchor(nID, CSize(MulDiv(rr.left,100,m_rrClient.Width()),MulDiv(rr.top,100,m_rrClient.Height())), CSize(MulDiv(rr.right,100,m_rrClient.Width()),MulDiv(rr.bottom,100,m_rrClient.Height())));	
}

void CPositionHoldingFokkerPage::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	CRect rr;
	LOGFONT	lf;

	if(GetSafeHwnd()) {
		GetClientRect(rr);
		lf = m_LogFont;
		lf.lfHeight = MulDiv(rr.Height(),m_LogFont.lfHeight,m_rrClient.Height());
		delete m_pFont;
		m_pFont = new CFont();
		m_pFont->CreateFontIndirect(&lf);
		m_editXMoving.SetFont(m_pFont);
		m_editYMoving.SetFont(m_pFont);
		m_editZMoving.SetFont(m_pFont);
		m_editXFixed.SetFont(m_pFont);
		m_editYFixed.SetFont(m_pFont);
		m_editZFixed.SetFont(m_pFont);
		m_staticXMoving.SetFont(m_pFont);
		m_staticYMoving.SetFont(m_pFont);
		m_staticZMoving.SetFont(m_pFont);
		m_staticXFixed.SetFont(m_pFont);
		m_staticYFixed.SetFont(m_pFont);
		m_staticZFixed.SetFont(m_pFont);
		m_staticFixed.SetFont(m_pFont);
		m_staticMoving.SetFont(m_pFont);
	}

	memset(nOldPos,0xcc,sizeof nOldPos);
	
	
}

void CPositionHoldingFokkerPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;
	CString Unit[] = {L"mm", L"\x022", L"\x0b0"};

	
	if(nOldPos[theApp.m_Tank.nXStep] - theApp.m_Axes[theApp.m_Tank.nXStep].nPos) {
		nOldPos[theApp.m_Tank.nXStep] = theApp.m_Axes[theApp.m_Tank.nXStep].nPos;
		MinMax(&theApp.m_Axes[theApp.m_Tank.nXStep].nUnitSelected,0,2);
		Buff.Format(_T("%.1f %s"),(float)theApp.m_Axes[theApp.m_Tank.nXStep].nPos * theApp.m_Axes[theApp.m_Tank.nXStep].fStepSize,Unit[theApp.m_Axes[theApp.m_Tank.nXStep].nUnitSelected]);
		m_editXMoving.SetWindowText(Buff);
	}
	if(nOldPos[theApp.m_Tank.nYStep] - theApp.m_Axes[theApp.m_Tank.nYStep].nPos) {
		nOldPos[theApp.m_Tank.nYStep] = theApp.m_Axes[theApp.m_Tank.nYStep].nPos;
		MinMax(&theApp.m_Axes[theApp.m_Tank.nYStep].nUnitSelected,0,2);
		Buff.Format(_T("%.1f %s"),(float)theApp.m_Axes[theApp.m_Tank.nYStep].nPos * theApp.m_Axes[theApp.m_Tank.nYStep].fStepSize,Unit[theApp.m_Axes[theApp.m_Tank.nYStep].nUnitSelected]);
		m_editYMoving.SetWindowText(Buff);
	}
	if(nOldPos[theApp.m_Tank.nZStep] - theApp.m_Axes[theApp.m_Tank.nZStep].nPos) {
		nOldPos[theApp.m_Tank.nZStep] = theApp.m_Axes[theApp.m_Tank.nZStep].nPos;
		MinMax(&theApp.m_Axes[theApp.m_Tank.nZStep].nUnitSelected,0,2);
		Buff.Format(_T("%.1f %s"),(float)theApp.m_Axes[theApp.m_Tank.nZStep].nPos * theApp.m_Axes[theApp.m_Tank.nZStep].fStepSize,Unit[theApp.m_Axes[theApp.m_Tank.nZStep].nUnitSelected]);
		m_editZMoving.SetWindowText(Buff);
	}
	if(nOldPos[theApp.m_Tank.nRStep] - theApp.m_Axes[theApp.m_Tank.nRStep].nPos) {
		nOldPos[theApp.m_Tank.nRStep] = theApp.m_Axes[theApp.m_Tank.nRStep].nPos;
		MinMax(&theApp.m_Axes[theApp.m_Tank.nRStep].nUnitSelected,0,2);
		Buff.Format(_T("%.1f %s"),(float)theApp.m_Axes[theApp.m_Tank.nRStep].nPos * theApp.m_Axes[theApp.m_Tank.nRStep].fStepSize,Unit[theApp.m_Axes[theApp.m_Tank.nRStep].nUnitSelected]);
		m_editXFixed.SetWindowText(Buff);
	}
	if(nOldPos[theApp.m_Tank.nXtStep] - theApp.m_Axes[theApp.m_Tank.nXtStep].nPos) {
		nOldPos[theApp.m_Tank.nXtStep] = theApp.m_Axes[theApp.m_Tank.nXtStep].nPos;
		MinMax(&theApp.m_Axes[theApp.m_Tank.nXtStep].nUnitSelected,0,2);
		Buff.Format(_T("%.1f %s"),(float)theApp.m_Axes[theApp.m_Tank.nXtStep].nPos * theApp.m_Axes[theApp.m_Tank.nXtStep].fStepSize,Unit[theApp.m_Axes[theApp.m_Tank.nXtStep].nUnitSelected]);
		m_editYFixed.SetWindowText(Buff);
	}
	if(nOldPos[theApp.m_Tank.nYtStep] - theApp.m_Axes[theApp.m_Tank.nYtStep].nPos) {
		nOldPos[theApp.m_Tank.nYtStep] = theApp.m_Axes[theApp.m_Tank.nYtStep].nPos;
		MinMax(&theApp.m_Axes[theApp.m_Tank.nYtStep].nUnitSelected,0,2);
		Buff.Format(_T("%.1f %s"),(float)theApp.m_Axes[theApp.m_Tank.nYtStep].nPos * theApp.m_Axes[theApp.m_Tank.nYtStep].fStepSize,Unit[theApp.m_Axes[theApp.m_Tank.nYtStep].nUnitSelected]);
		m_editZFixed.SetWindowText(Buff);
	}

	CResizablePage::OnTimer(nIDEvent);
}


inline void CPositionHoldingFokkerPage::MinMax(int *nn, int nMin, int nMax)
{
	if(*nn<nMin) *nn=nMin;
	if(*nn>nMax) *nn=nMax;
}
