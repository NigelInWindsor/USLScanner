// RobotDualPosPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RobotDualPosPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRobotDualPosPage property page

IMPLEMENT_DYNCREATE(CRobotDualPosPage, CResizablePage)

CRobotDualPosPage::CRobotDualPosPage() : CResizablePage(CRobotDualPosPage::IDD)
{
	//{{AFX_DATA_INIT(CRobotDualPosPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pFont = NULL;
}

CRobotDualPosPage::~CRobotDualPosPage()
{
	SAFE_DELETE( m_pFont );
}

void CRobotDualPosPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRobotDualPosPage)
	DDX_Control(pDX, IDC_STATIC_ZRIGHT, m_staticZRight);
	DDX_Control(pDX, IDC_STATIC_ZLEFT, m_staticZLeft);
	DDX_Control(pDX, IDC_STATIC_YRIGHT, m_staticYRight);
	DDX_Control(pDX, IDC_STATIC_YLEFT, m_staticYLeft);
	DDX_Control(pDX, IDC_STATIC_XRIGHT, m_staticXRight);
	DDX_Control(pDX, IDC_STATIC_XLEFT, m_staticXLeft);
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_staticRight);
	DDX_Control(pDX, IDC_STATIC_LEFT, m_staticLeft);
	DDX_Control(pDX, IDC_STATIC_KRIGHT, m_staticKRight);
	DDX_Control(pDX, IDC_STATIC_KLEFT, m_staticKLeft);
	DDX_Control(pDX, IDC_STATIC_JRIGHT, m_staticJRight);
	DDX_Control(pDX, IDC_STATIC_JLEFT, m_staticJLeft);
	DDX_Control(pDX, IDC_STATIC_IRIGHT, m_staticIRight);
	DDX_Control(pDX, IDC_STATIC_ILEFT, m_staticILeft);
	DDX_Control(pDX, IDC_EDIT_POS_ZRIGHT, m_editZRight);
	DDX_Control(pDX, IDC_EDIT_POS_ZLEFT, m_editZLeft);
	DDX_Control(pDX, IDC_EDIT_POS_YRIGHT, m_editYRight);
	DDX_Control(pDX, IDC_EDIT_POS_YLEFT, m_editYLeft);
	DDX_Control(pDX, IDC_EDIT_POS_JRIGHT, m_editJRight);
	DDX_Control(pDX, IDC_EDIT_POS_IRIGHT, m_editIRight);
	DDX_Control(pDX, IDC_EDIT_POS_XRIGHT, m_editXRight);
	DDX_Control(pDX, IDC_EDIT_POS_XLEFT, m_editXLeft);
	DDX_Control(pDX, IDC_EDIT_POS_KRIGHT, m_editKRight);
	DDX_Control(pDX, IDC_EDIT_POS_KLEFT, m_editKLeft);
	DDX_Control(pDX, IDC_EDIT_POS_JLEFT, m_editJLeft);
	DDX_Control(pDX, IDC_EDIT_POS_ILEFT, m_editILeft);
	DDX_Control(pDX, IDC_COMBO_POSITION_MODE, m_comboDisplayMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRobotDualPosPage, CResizablePage)
	//{{AFX_MSG_MAP(CRobotDualPosPage)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_POSITION_MODE, OnSelchangeComboPositionMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotDualPosPage message handlers

BOOL CRobotDualPosPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	CString Buff;

	m_nOldMeasurementUnits = theApp.m_LastSettings.nMeasurementUnits;

	m_staticLeft.SetWindowText(theApp.m_DlgSideName[0]);
	m_staticRight.SetWindowText(theApp.m_DlgSideName[1]);

	GetClientRect(m_rrClient);

	AddAnchor(&m_editXRight);
	AddAnchor(&m_editYRight);
	AddAnchor(&m_editZRight);
	AddAnchor(&m_editIRight);
	AddAnchor(&m_editJRight);
	AddAnchor(&m_editKRight);
	AddAnchor(&m_editXLeft);
	AddAnchor(&m_editYLeft);
	AddAnchor(&m_editZLeft);
	AddAnchor(&m_editILeft);
	AddAnchor(&m_editJLeft);
	AddAnchor(&m_editKLeft);
	AddAnchor(&m_staticXRight);
	AddAnchor(&m_staticYRight);
	AddAnchor(&m_staticZRight);
	AddAnchor(&m_staticIRight);
	AddAnchor(&m_staticJRight);
	AddAnchor(&m_staticKRight);
	AddAnchor(&m_staticXLeft);
	AddAnchor(&m_staticYLeft);
	AddAnchor(&m_staticZLeft);
	AddAnchor(&m_staticILeft);
	AddAnchor(&m_staticJLeft);
	AddAnchor(&m_staticKLeft);
	AddAnchor(&m_staticRight);
	AddAnchor(&m_staticLeft);
	AddAnchor(&m_comboDisplayMode);

	Buff.LoadString(IDS_Joint);							m_comboDisplayMode.AddString(Buff);
	Buff.LoadString(IDS_Head);							m_comboDisplayMode.AddString(Buff);
	Buff.LoadString(IDS_Surface);						m_comboDisplayMode.AddString(Buff);
	Buff.LoadString(IDS_Surface_Water);					m_comboDisplayMode.AddString(Buff);
	Buff.LoadString(IDS_Surface_without_transporter);	m_comboDisplayMode.AddString(Buff);
	m_comboDisplayMode.SetCurSel(theApp.m_LastSettings.nPositionDisplayMode);


	CFont* pFont = GetFont();
	pFont->GetLogFont(&m_LogFont);

	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&m_LogFont);
	

	SetStaticNames();


	SetTimer(1,300,NULL);
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CRobotDualPosPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CRobotDualPosPage::AddAnchor(CWnd *pCtrl)
{
	CRect rr;

	pCtrl->GetWindowRect(&rr);
	ScreenToClient(&rr);

	int nID = pCtrl->GetDlgCtrlID();
	CResizablePage::AddAnchor(nID, CSize(MulDiv(rr.left,100,m_rrClient.Width()),MulDiv(rr.top,100,m_rrClient.Height())), CSize(MulDiv(rr.right,100,m_rrClient.Width()),MulDiv(rr.bottom,100,m_rrClient.Height())));	
}

void CRobotDualPosPage::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	CRect rr,r;
	LOGFONT	lf;
	CString Buff;

	if(GetSafeHwnd() && m_editXRight.GetSafeHwnd()) {
		
		GetClientRect(rr);
		lf = m_LogFont;
		lf.lfHeight = MulDiv(rr.Height(),m_LogFont.lfHeight,m_rrClient.Height());
		SAFE_DELETE( m_pFont );
		m_pFont = new CFont();
		m_pFont->CreateFontIndirect(&lf);
		m_editXRight.SetFont(m_pFont);
		m_editYRight.SetFont(m_pFont);
		m_editZRight.SetFont(m_pFont);
		m_editIRight.SetFont(m_pFont);
		m_editJRight.SetFont(m_pFont);
		m_editKRight.SetFont(m_pFont);
		m_editXLeft.SetFont(m_pFont);
		m_editYLeft.SetFont(m_pFont);
		m_editZLeft.SetFont(m_pFont);
		m_editILeft.SetFont(m_pFont);
		m_editJLeft.SetFont(m_pFont);
		m_editKLeft.SetFont(m_pFont);
		m_staticXRight.SetFont(m_pFont);
		m_staticYRight.SetFont(m_pFont);
		m_staticZRight.SetFont(m_pFont);
		m_staticIRight.SetFont(m_pFont);
		m_staticJRight.SetFont(m_pFont);
		m_staticKRight.SetFont(m_pFont);
		m_staticXLeft.SetFont(m_pFont);
		m_staticYLeft.SetFont(m_pFont);
		m_staticZLeft.SetFont(m_pFont);
		m_staticILeft.SetFont(m_pFont);
		m_staticJLeft.SetFont(m_pFont);
		m_staticKLeft.SetFont(m_pFont);
		m_staticRight.SetFont(m_pFont);
		m_staticLeft.SetFont(m_pFont);
		m_comboDisplayMode.SetFont(m_pFont);
		
	}
}

void CRobotDualPosPage::OnTimer(UINT nIDEvent) 
{
	CCoord CpSurface;
	CString Unit[] = {L"mm", L"\x022", L"\x0b0"};
	CString Buff;


	if(	m_nOldMeasurementUnits - theApp.m_LastSettings.nMeasurementUnits) {
		m_nOldMeasurementUnits = theApp.m_LastSettings.nMeasurementUnits;
		m_CpOld.Zero();
	}


	switch(theApp.m_LastSettings.nPositionDisplayMode ) {
	case 0:
		theApp.m_Robot[0].GetJointPos(&CpSurface,PORTSIDE);
		theApp.m_Robot[1].GetJointPos(&CpSurface,STARBOARD);

		switch(theApp.m_nSide0Orientation) {
		case 0:
			if(m_CpOld.Side0.fX - CpSurface.Side0.fX) {
				Buff.Format(_T("%.02f %s"),CpSurface.Side0.fPos[0] * RAD_TO_DEG, DEGREES);
				m_editXLeft.SetWindowText(Buff);
			}
			if(m_CpOld.Side0.fY - CpSurface.Side0.fY) {
				Buff.Format(_T("%.02f %s"),CpSurface.Side0.fPos[1] * RAD_TO_DEG, DEGREES);
				m_editYLeft.SetWindowText(Buff);
			}
			if(m_CpOld.Side0.fZ - CpSurface.Side0.fZ) {
				Buff.Format(_T("%.02f %s"),CpSurface.Side0.fPos[2] * RAD_TO_DEG, DEGREES);
				m_editZLeft.SetWindowText(Buff);
			}
			if(m_CpOld.Side0.fXt - CpSurface.Side0.fXt) {
				Buff.Format(_T("%.02f %s"),CpSurface.Side0.fPos[3] * RAD_TO_DEG, DEGREES);
				m_editILeft.SetWindowText(Buff);
			}
			if(m_CpOld.Side0.fYt - CpSurface.Side0.fYt) {
				Buff.Format(_T("%.02f %s"),CpSurface.Side0.fPos[4] * RAD_TO_DEG, DEGREES);
				m_editJLeft.SetWindowText(Buff);
			}
			if(m_CpOld.Side0.fR - CpSurface.Side0.fR) {
				Buff.Format(_T("%.02f %s"),CpSurface.Side0.fPos[5] * RAD_TO_DEG, DEGREES);
				m_editKLeft.SetWindowText(Buff);
			}
			if(m_CpOld.Side1.fX - CpSurface.Side1.fX) {
				Buff.Format(_T("%.02f %s"),CpSurface.Side1.fPos[0] * RAD_TO_DEG, DEGREES);
				m_editXRight.SetWindowText(Buff);
			}
			if(m_CpOld.Side1.fY - CpSurface.Side1.fY) {
				Buff.Format(_T("%.02f %s"),CpSurface.Side1.fPos[1] * RAD_TO_DEG, DEGREES);
				m_editYRight.SetWindowText(Buff);
			}
			if(m_CpOld.Side1.fZ - CpSurface.Side1.fZ) {
				Buff.Format(_T("%.02f %s"),CpSurface.Side1.fPos[2] * RAD_TO_DEG, DEGREES);
				m_editZRight.SetWindowText(Buff);
			}
			if(m_CpOld.Side1.fXt - CpSurface.Side1.fXt) {
				Buff.Format(_T("%.02f %s"),CpSurface.Side1.fPos[3] * RAD_TO_DEG, DEGREES);
				m_editIRight.SetWindowText(Buff);
			}
			if(m_CpOld.Side1.fYt - CpSurface.Side1.fYt) {
				Buff.Format(_T("%.02f %s"),CpSurface.Side1.fPos[4] * RAD_TO_DEG, DEGREES);
				m_editJRight.SetWindowText(Buff);
			}
			if(m_CpOld.Side1.fR - CpSurface.Side1.fR) {
				Buff.Format(_T("%.02f %s"),CpSurface.Side1.fPos[5] * RAD_TO_DEG, DEGREES);
				m_editKRight.SetWindowText(Buff);
			}
			break;
		case 1:
			break;
		}
		break;
	case 1:
		theApp.m_Robot[0].GetToolPos(&CpSurface,PORTSIDE);
		theApp.m_Robot[1].GetToolPos(&CpSurface,STARBOARD);
		break;
	case 2:
	case 3:
		theApp.m_Motors.GetSurfacePos(&CpSurface);
		break;
	case 4:
		CpSurface.Zero();
		break;
	}

	switch(theApp.m_nSide0Orientation) {
	case 0:
		if(m_CpOld.Side0.fX - CpSurface.Side0.fX) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPrecision,CpSurface.Side0.fX * theApp.m_fUnits, theApp.m_Units);
			m_editXLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fY - CpSurface.Side0.fY) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPrecision,CpSurface.Side0.fY * theApp.m_fUnits, theApp.m_Units);
			m_editYLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fZ - CpSurface.Side0.fZ) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPrecision,CpSurface.Side0.fZ * theApp.m_fUnits, theApp.m_Units);
			m_editZLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fXt - CpSurface.Side0.fXt) {
			Buff.Format(_T("%.03f"),CpSurface.Side0.fI);
			m_editILeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fYt - CpSurface.Side0.fYt) {
			Buff.Format(_T("%.03f"),CpSurface.Side0.fJ);
			m_editJLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fR - CpSurface.Side0.fR) {
			Buff.Format(_T("%.03f"),CpSurface.Side0.fK);
			m_editKLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side1.fX - CpSurface.Side1.fX) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXRight].nPrecision,CpSurface.Side1.fX * theApp.m_fUnits, theApp.m_Units);
			m_editXRight.SetWindowText(Buff);
		}
		if(m_CpOld.Side1.fY - CpSurface.Side1.fY) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYRight].nPrecision,CpSurface.Side1.fY * theApp.m_fUnits, theApp.m_Units);
			m_editYRight.SetWindowText(Buff);
		}
		if(m_CpOld.Side1.fZ - CpSurface.Side1.fZ) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZRight].nPrecision,CpSurface.Side1.fZ * theApp.m_fUnits, theApp.m_Units);
			m_editZRight.SetWindowText(Buff);
		}
		if(m_CpOld.Side1.fXt - CpSurface.Side1.fXt) {
			Buff.Format(_T("%.03f"),CpSurface.Side1.fI);
			m_editIRight.SetWindowText(Buff);
		}
		if(m_CpOld.Side1.fYt - CpSurface.Side1.fYt) {
			Buff.Format(_T("%.03f"),CpSurface.Side1.fJ);
			m_editJRight.SetWindowText(Buff);
		}
		if(m_CpOld.Side1.fR - CpSurface.Side1.fR) {
			Buff.Format(_T("%.03f"),CpSurface.Side1.fK);
			m_editKRight.SetWindowText(Buff);
		}
		break;
	case 1:
		break;
	}

		
	CResizablePage::OnTimer(nIDEvent);
}

void CRobotDualPosPage::SetStaticNames()
{
	switch(theApp.m_LastSettings.nPositionDisplayMode) {
	case 0:
		m_staticXLeft.SetWindowText(L"J1");
		m_staticYLeft.SetWindowText(L"J2");
		m_staticZLeft.SetWindowText(L"J3");
		m_staticILeft.SetWindowText(L"J4");
		m_staticJLeft.SetWindowText(L"J5");
		m_staticKLeft.SetWindowText(L"J6");
		m_staticXRight.SetWindowText(L"J1");
		m_staticYRight.SetWindowText(L"J2");
		m_staticZRight.SetWindowText(L"J3");
		m_staticIRight.SetWindowText(L"J4");
		m_staticJRight.SetWindowText(L"J5");
		m_staticKRight.SetWindowText(L"J6");
		break;
	default:
		m_staticXLeft.SetWindowText(L"X");
		m_staticYLeft.SetWindowText(L"Y");
		m_staticZLeft.SetWindowText(L"Z");
		m_staticILeft.SetWindowText(L"I");
		m_staticJLeft.SetWindowText(L"J");
		m_staticKLeft.SetWindowText(L"K");
		m_staticXRight.SetWindowText(L"X");
		m_staticYRight.SetWindowText(L"Y");
		m_staticZRight.SetWindowText(L"Z");
		m_staticIRight.SetWindowText(L"I");
		m_staticJRight.SetWindowText(L"J");
		m_staticKRight.SetWindowText(L"K");
		break;
	};
}

void CRobotDualPosPage::OnSelchangeComboPositionMode() 
{
	theApp.m_LastSettings.nPositionDisplayMode = m_comboDisplayMode.GetCurSel();
	SetStaticNames();	
}
