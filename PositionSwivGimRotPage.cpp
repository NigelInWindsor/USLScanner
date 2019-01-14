// PositionSwivGimRotPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "PositionSwivGimRotPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPositionSwivGimRotPage dialog

IMPLEMENT_DYNCREATE(CPositionSwivGimRotPage, CResizablePage)

CPositionSwivGimRotPage::CPositionSwivGimRotPage() : CResizablePage(CPositionSwivGimRotPage::IDD)
{
	//{{AFX_DATA_INIT(CPositionSingleBridgePage)
	//}}AFX_DATA_INIT
	
	m_pFont = FALSE;
}

CPositionSwivGimRotPage::~CPositionSwivGimRotPage()
{
	SAFE_DELETE( m_pFont );

}



void CPositionSwivGimRotPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPositionSwivGimRotPage)
	DDX_Control(pDX, IDC_STATIC_AXIS, m_buttonAxis);
	DDX_Control(pDX, IDC_STATIC_Z, m_staticZ);
	DDX_Control(pDX, IDC_STATIC_Y, m_staticY);
	DDX_Control(pDX, IDC_STATIC_X, m_staticX);
	DDX_Control(pDX, IDC_STATIC_R, m_staticR);
	DDX_Control(pDX, IDC_STATIC_SWIVAL, m_staticSwival);
	DDX_Control(pDX, IDC_STATIC_GIMBAL, m_staticGimbal);
	DDX_Control(pDX, IDC_EDIT_AXIS_5, m_editAxis5);
	DDX_Control(pDX, IDC_EDIT_AXIS_4, m_editAxis4);
	DDX_Control(pDX, IDC_EDIT_AXIS_3, m_editAxis3);
	DDX_Control(pDX, IDC_EDIT_AXIS_2, m_editAxis2);
	DDX_Control(pDX, IDC_EDIT_AXIS_1, m_editAxis1);
	DDX_Control(pDX, IDC_EDIT_AXIS_0, m_editAxis0);
	DDX_Control(pDX, IDC_COMBO_POSITION_MODE, m_comboPositionMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPositionSwivGimRotPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPositionSwivGimRotPage)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_POSITION_MODE, OnSelchangeComboPositionMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPositionSwivGimRotPage message handlers

BOOL CPositionSwivGimRotPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	CString Buff;

	m_nOldMeasurementUnits = theApp.m_LastSettings.nMeasurementUnits;

	GetClientRect(m_rrClient);

	AddAnchor(&m_editAxis0);
	AddAnchor(&m_editAxis1);
	AddAnchor(&m_editAxis2);
	AddAnchor(&m_editAxis3);
	AddAnchor(&m_editAxis4);
	AddAnchor(&m_editAxis5);
	AddAnchor(&m_staticX);
	AddAnchor(&m_staticY);
	AddAnchor(&m_staticZ);
	AddAnchor(&m_staticR);
	AddAnchor(&m_staticSwival);
	AddAnchor(&m_staticGimbal);
	AddAnchor(&m_buttonAxis);

	AddAnchor(&m_comboPositionMode);

	Buff.LoadString(IDS_Machine);		m_comboPositionMode.AddString(Buff);
	Buff.LoadString(IDS_Head);			m_comboPositionMode.AddString(Buff);
	Buff.LoadString(IDS_Surface);		m_comboPositionMode.AddString(Buff);
	Buff.LoadString(IDS_Surface_Water); m_comboPositionMode.AddString(Buff);


	CFont* pFont = GetFont();
	pFont->GetLogFont(&m_LogFont);

	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&m_LogFont);
	
	UpdateAllControls();

	SetTimer(1,300,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPositionSwivGimRotPage::AddAnchor(CWnd *pCtrl)
{
	CRect rr;

	pCtrl->GetWindowRect(&rr);
	ScreenToClient(&rr);

	int nID = pCtrl->GetDlgCtrlID();
	CResizablePage::AddAnchor(nID, CSize(MulDiv(rr.left,100,m_rrClient.Width()),MulDiv(rr.top,100,m_rrClient.Height())), CSize(MulDiv(rr.right,100,m_rrClient.Width()),MulDiv(rr.bottom,100,m_rrClient.Height())));	
}

void CPositionSwivGimRotPage::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	CRect rr,r;
	LOGFONT	lf;
	CString Buff;

	if(GetSafeHwnd()) {
		
		GetClientRect(rr);
		lf = m_LogFont;
		lf.lfHeight = MulDiv(rr.Height(),m_LogFont.lfHeight,m_rrClient.Height());
		delete m_pFont;
		m_pFont = new CFont();
		m_pFont->CreateFontIndirect(&lf);

		m_editAxis0.SetFont(m_pFont);
		m_editAxis1.SetFont(m_pFont);
		m_editAxis2.SetFont(m_pFont);
		m_editAxis3.SetFont(m_pFont);
		m_editAxis4.SetFont(m_pFont);
		m_editAxis5.SetFont(m_pFont);
		m_staticX.SetFont(m_pFont);
		m_staticY.SetFont(m_pFont);
		m_staticZ.SetFont(m_pFont);
		m_staticR.SetFont(m_pFont);
		m_staticSwival.SetFont(m_pFont);
		m_staticGimbal.SetFont(m_pFont);
		m_buttonAxis.SetFont(m_pFont);
		m_comboPositionMode.SetFont(m_pFont);
	}

}

void CPositionSwivGimRotPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;
	CString Unit[] = {L"mm", L"\x022", L"\x0b0"};
	CCoord CpHead,CpSurface,CpAxes;

	if(	m_nOldMeasurementUnits - theApp.m_LastSettings.nMeasurementUnits) {
		m_nOldMeasurementUnits = theApp.m_LastSettings.nMeasurementUnits;
		m_CpOld.Zero();
	}


	switch(theApp.m_LastSettings.nPositionDisplayMode) {
	case 0: theApp.m_Motors.GetEncoderPos(&CpSurface);
		break;
	case 1:	theApp.m_Motors.GetHeadPos(&CpSurface);
		break;
	case 2:	theApp.m_Motors.GetHeadPos(&CpHead);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
		break;
	case 3:	theApp.m_Motors.GetHeadPos(&CpHead);
		theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
		theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,FALSE);
		break;
	}
//	theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
	switch(theApp.m_LastSettings.nPositionDisplayMode) {
	case 0:
		if(m_CpOld.Side0.fX - CpSurface.Side0.fX) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPrecision,CpSurface.Side0.fX * theApp.m_fUnits, theApp.m_Units);
			m_editAxis0.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fY - CpSurface.Side0.fY) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPrecision,CpSurface.Side0.fY * theApp.m_fUnits, theApp.m_Units);
			m_editAxis1.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fZ - CpSurface.Side0.fZ) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPrecision,CpSurface.Side0.fZ * theApp.m_fUnits, theApp.m_Units);
			m_editAxis2.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fR - CpSurface.Side0.fR) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPrecision,CpSurface.Side0.fR,DEGREES);
			m_editAxis3.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fXt - CpSurface.Side0.fXt) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPrecision,CpSurface.Side0.fXt,DEGREES);
			m_editAxis4.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fYt - CpSurface.Side0.fYt) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPrecision,CpSurface.Side0.fYt,DEGREES);
			m_editAxis5.SetWindowText(Buff);
		}
		break;
	case 1:
	case 2:
	case 3:
		if(m_CpOld.Side0.fX - CpSurface.Side0.fX) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPrecision,CpSurface.Side0.fX * theApp.m_fUnits, theApp.m_Units);
			m_editAxis0.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fY - CpSurface.Side0.fY) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPrecision,CpSurface.Side0.fY * theApp.m_fUnits, theApp.m_Units);
			m_editAxis1.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fZ - CpSurface.Side0.fZ) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPrecision,CpSurface.Side0.fZ * theApp.m_fUnits, theApp.m_Units);
			m_editAxis2.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fI - CpSurface.Side0.fI) {
			Buff.Format(_T("%.03f"),CpSurface.Side0.fI);
			m_editAxis3.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fJ - CpSurface.Side0.fJ) {
			Buff.Format(_T("%.03f"),CpSurface.Side0.fJ);
			m_editAxis4.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fK - CpSurface.Side0.fK) {
			Buff.Format(_T("%.03f"),CpSurface.Side0.fK);
			m_editAxis5.SetWindowText(Buff);
		}
		break;
	}

	m_CpOld = CpSurface;
	
	CPropertyPage::OnTimer(nIDEvent);
}

void CPositionSwivGimRotPage::OnSelchangeComboPositionMode() 
{
	theApp.m_LastSettings.nPositionDisplayMode =m_comboPositionMode.GetCurSel();
	UpdateAllControls();
}

void CPositionSwivGimRotPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	m_comboPositionMode.SetCurSel(theApp.m_LastSettings.nPositionDisplayMode);

	switch(theApp.m_LastSettings.nPositionDisplayMode) {
	case 0:
		m_staticX.SetWindowText(L"X");
		m_staticY.SetWindowText(L"Y");
		m_staticZ.SetWindowText(L"Z");
		m_staticR.SetWindowText(L"Rotate");
		m_staticSwival.SetWindowText(L"Swivel");
		m_staticGimbal.SetWindowText(L"Gimbal");
		break;
	case 1:
	case 2:
	case 3:
		m_staticX.SetWindowText(L"X");
		m_staticY.SetWindowText(L"Y");
		m_staticZ.SetWindowText(L"Z");
		m_staticR.SetWindowText(L"I");
		m_staticSwival.SetWindowText(L"J");
		m_staticGimbal.SetWindowText(L"K");
		break;
	}

}
