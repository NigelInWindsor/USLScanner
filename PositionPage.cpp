// PositionFokkerPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PositionPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPositionPage property page

IMPLEMENT_DYNCREATE(CPositionPage, CResizablePage)

CPositionPage::CPositionPage() : CResizablePage(CPositionPage::IDD)
{
	//{{AFX_DATA_INIT(CPositionPage)
	//}}AFX_DATA_INIT
	
	m_pFont = NULL;
}

CPositionPage::~CPositionPage()
{
	SAFE_DELETE( m_pFont );
}

void CPositionPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPositionPage)
	DDX_Control(pDX, IDC_STATIC_R, m_staticR);
	DDX_Control(pDX, IDC_STATIC_BRIDGE, m_staticBridge);
	DDX_Control(pDX, IDC_EDIT_POS_R, m_editPosR);
	DDX_Control(pDX, IDC_EDIT_POS_BRIDGE, m_editPosBridge);
	DDX_Control(pDX, IDC_COMBO_POSITION_MODE, m_comboPositionMode);
	DDX_Control(pDX, IDC_EDIT_POS_XRIGHT, m_editPosXRight);
	DDX_Control(pDX, IDC_EDIT_POS_YRIGHT, m_editPosYRight);
	DDX_Control(pDX, IDC_EDIT_POS_ZRIGHT, m_editPosZRight);
	DDX_Control(pDX, IDC_EDIT_POS_XTRIGHT, m_editPosXtRight);
	DDX_Control(pDX, IDC_EDIT_POS_ZTRIGHT, m_editPosZtRight);
	DDX_Control(pDX, IDC_EDIT_POS_XLEFT, m_editPosXLeft);
	DDX_Control(pDX, IDC_EDIT_POS_YLEFT, m_editPosYLeft);
	DDX_Control(pDX, IDC_EDIT_POS_ZLEFT, m_editPosZLeft);
	DDX_Control(pDX, IDC_EDIT_POS_XTLEFT, m_editPosXtLeft);
	DDX_Control(pDX, IDC_EDIT_POS_ZTLEFT, m_editPosZtLeft);
	DDX_Control(pDX, IDC_STATIC_XRIGHT, m_staticXRight);
	DDX_Control(pDX, IDC_STATIC_YRIGHT, m_staticYRight);
	DDX_Control(pDX, IDC_STATIC_ZRIGHT, m_staticZRight);
	DDX_Control(pDX, IDC_STATIC_XTRIGHT, m_staticXtRight);
	DDX_Control(pDX, IDC_STATIC_ZTRIGHT, m_staticZtRight);
	DDX_Control(pDX, IDC_STATIC_XLEFT, m_staticXLeft);
	DDX_Control(pDX, IDC_STATIC_YLEFT, m_staticYLeft);
	DDX_Control(pDX, IDC_STATIC_ZLEFT, m_staticZLeft);
	DDX_Control(pDX, IDC_STATIC_XTLEFT, m_staticXtLeft);
	DDX_Control(pDX, IDC_STATIC_ZTLEFT, m_staticZtLeft);
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_staticRight);
	DDX_Control(pDX, IDC_STATIC_LEFT, m_staticLeft);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPositionPage, CResizablePage)
	//{{AFX_MSG_MAP(CPositionPage)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_POSITION_MODE, OnSelchangeComboPositionMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPositionPage message handlers

BOOL CPositionPage::OnInitDialog() 
{
	
	CResizablePage::OnInitDialog();
	CString Buff;

	m_nOldMeasurementUnits = theApp.m_LastSettings.nMeasurementUnits;

	m_staticLeft.SetWindowText(theApp.m_DlgSideName[0]);
	m_staticRight.SetWindowText(theApp.m_DlgSideName[1]);

	GetClientRect(m_rrClient);

	AddAnchor(&m_editPosXRight);
	AddAnchor(&m_editPosYRight);
	AddAnchor(&m_editPosZRight);
	AddAnchor(&m_editPosXtRight);
	AddAnchor(&m_editPosZtRight);
	AddAnchor(&m_editPosXLeft);
	AddAnchor(&m_editPosYLeft);
	AddAnchor(&m_editPosZLeft);
	AddAnchor(&m_editPosXtLeft);
	AddAnchor(&m_editPosZtLeft);
	AddAnchor(&m_editPosBridge);
	AddAnchor(&m_editPosR);
	AddAnchor(&m_staticXRight);
	AddAnchor(&m_staticYRight);
	AddAnchor(&m_staticZRight);
	AddAnchor(&m_staticXtRight);
	AddAnchor(&m_staticZtRight);
	AddAnchor(&m_staticXLeft);
	AddAnchor(&m_staticYLeft);
	AddAnchor(&m_staticZLeft);
	AddAnchor(&m_staticXtLeft);
	AddAnchor(&m_staticZtLeft);
	AddAnchor(&m_staticRight);
	AddAnchor(&m_staticLeft);
	AddAnchor(&m_staticBridge);
	AddAnchor(&m_staticR);
	AddAnchor(&m_comboPositionMode);

	Buff.LoadString(IDS_Machine);		m_comboPositionMode.AddString(Buff);
	Buff.LoadString(IDS_Head);			m_comboPositionMode.AddString(Buff);
	Buff.LoadString(IDS_Surface);		m_comboPositionMode.AddString(Buff);
	Buff.LoadString(IDS_Surface_Water); m_comboPositionMode.AddString(Buff);
	m_comboPositionMode.SetCurSel(theApp.m_LastSettings.nPositionDisplayMode);


	CFont* pFont = GetFont();
	pFont->GetLogFont(&m_LogFont);

	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&m_LogFont);
	

	if(theApp.m_nJobNumber!=GKN){
		//Hide bridge and R options
		m_staticR.ModifyStyle(WS_VISIBLE,0);
		m_staticBridge.ModifyStyle(WS_VISIBLE,0);
		m_editPosBridge.ModifyStyle(WS_VISIBLE,0);
		m_editPosR.ModifyStyle(WS_VISIBLE,0);

	}
//	theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable ? bFlagShow = true : bFlagShow = false;
	if(theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable==false)	m_editPosXLeft.ModifyStyle(WS_VISIBLE,0);
	if(theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable==false)	m_editPosYLeft.ModifyStyle(WS_VISIBLE,0);
	if(theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable==false)	m_editPosZLeft.ModifyStyle(WS_VISIBLE,0);
	if(theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable==false)	m_editPosXtLeft.ModifyStyle(WS_VISIBLE,0);
	if(theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==false)	m_editPosZtLeft.ModifyStyle(WS_VISIBLE,0);

	if(theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==false)	m_editPosXRight.ModifyStyle(WS_VISIBLE,0);
	if(theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==false)	m_editPosYRight.ModifyStyle(WS_VISIBLE,0);
	if(theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==false)	m_editPosZRight.ModifyStyle(WS_VISIBLE,0);
	if(theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==false)	m_editPosXtRight.ModifyStyle(WS_VISIBLE,0);
	if(theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==false)	m_editPosZtRight.ModifyStyle(WS_VISIBLE,0);


	m_staticXLeft.SetWindowText(theApp.m_Axes[theApp.m_Tank.nXLeft].cName);
	m_staticYLeft.SetWindowText(theApp.m_Axes[theApp.m_Tank.nYLeft].cName);
	m_staticZLeft.SetWindowText(theApp.m_Axes[theApp.m_Tank.nZLeft].cName);
	m_staticXtLeft.SetWindowText(theApp.m_Axes[theApp.m_Tank.nXtLeft].cName);
	m_staticZtLeft.SetWindowText(theApp.m_Axes[theApp.m_Tank.nYtLeft].cName);
	m_staticXRight.SetWindowText(theApp.m_Axes[theApp.m_Tank.nXRight].cName);
	m_staticYRight.SetWindowText(theApp.m_Axes[theApp.m_Tank.nYRight].cName);
	m_staticZRight.SetWindowText(theApp.m_Axes[theApp.m_Tank.nZRight].cName);
	m_staticXtRight.SetWindowText(theApp.m_Axes[theApp.m_Tank.nXtRight].cName);
	m_staticZtRight.SetWindowText(theApp.m_Axes[theApp.m_Tank.nYtRight].cName);


	if(theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable==false)	m_staticXRight.ModifyStyle(WS_VISIBLE,0);
	if(theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable==false)	m_staticYLeft.ModifyStyle(WS_VISIBLE,0);
	if(theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable==false)	m_staticZLeft.ModifyStyle(WS_VISIBLE,0);
	if(theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable==false)	m_staticXtLeft.ModifyStyle(WS_VISIBLE,0);
	if(theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==false)	m_staticZtLeft.ModifyStyle(WS_VISIBLE,0);

	if(theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==false)	m_staticXLeft.ModifyStyle(WS_VISIBLE,0);
	if(theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==false)	m_staticYRight.ModifyStyle(WS_VISIBLE,0);
	if(theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==false)	m_staticZRight.ModifyStyle(WS_VISIBLE,0);
	if(theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==false)	m_staticXtRight.ModifyStyle(WS_VISIBLE,0);
	if(theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==false)	m_staticZtRight.ModifyStyle(WS_VISIBLE,0);

	SetTimer(1,300,NULL);

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPositionPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CPositionPage::AddAnchor(CWnd *pCtrl)
{
	CRect rr;

	pCtrl->GetWindowRect(&rr);
	ScreenToClient(&rr);

	int nID = pCtrl->GetDlgCtrlID();
	CResizablePage::AddAnchor(nID, CSize(MulDiv(rr.left,100,m_rrClient.Width()),MulDiv(rr.top,100,m_rrClient.Height())), CSize(MulDiv(rr.right,100,m_rrClient.Width()),MulDiv(rr.bottom,100,m_rrClient.Height())));	
}

void CPositionPage::OnSize(UINT nType, int cx, int cy) 
{
	
	CResizablePage::OnSize(nType, cx, cy);
	CRect rr,r;
	LOGFONT	lf;
	CString Buff;

	if(GetSafeHwnd() && m_editPosXRight.GetSafeHwnd()) {
		
		GetClientRect(rr);
		lf = m_LogFont;
		lf.lfHeight = MulDiv(rr.Height(),m_LogFont.lfHeight,m_rrClient.Height());
		SAFE_DELETE( m_pFont );
		m_pFont = new CFont();
		m_pFont->CreateFontIndirect(&lf);
		m_editPosXRight.SetFont(m_pFont);
		m_editPosYRight.SetFont(m_pFont);
		m_editPosZRight.SetFont(m_pFont);
		m_editPosXtRight.SetFont(m_pFont);
		m_editPosZtRight.SetFont(m_pFont);
		m_editPosXLeft.SetFont(m_pFont);
		m_editPosYLeft.SetFont(m_pFont);
		m_editPosZLeft.SetFont(m_pFont);
		m_editPosXtLeft.SetFont(m_pFont);
		m_editPosZtLeft.SetFont(m_pFont);
		m_editPosBridge.SetFont(m_pFont);
		m_editPosR.SetFont(m_pFont);
		m_staticXRight.SetFont(m_pFont);
		m_staticYRight.SetFont(m_pFont);
		m_staticZRight.SetFont(m_pFont);
		m_staticXtRight.SetFont(m_pFont);
		m_staticZtRight.SetFont(m_pFont);
		m_staticXLeft.SetFont(m_pFont);
		m_staticYLeft.SetFont(m_pFont);
		m_staticZLeft.SetFont(m_pFont);
		m_staticXtLeft.SetFont(m_pFont);
		m_staticZtLeft.SetFont(m_pFont);
		m_staticRight.SetFont(m_pFont);
		m_staticLeft.SetFont(m_pFont);
		m_staticBridge.SetFont(m_pFont);
		m_staticR.SetFont(m_pFont);
		m_comboPositionMode.SetFont(m_pFont);
		
	}

}

void CPositionPage::OnTimer(UINT nIDEvent) 
{
	
	CString Buff;
	CString Unit[] = {L"mm", L"\x022", L"\x0b0"};
	CCoord CpHead,CpSurface;

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
	
	switch(theApp.m_nSide0Orientation) {
	case 0:
		if(m_CpOld.Side0.fX - CpSurface.Side0.fX) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPrecision,CpSurface.Side0.fX * theApp.m_fUnits, theApp.m_Units);
			m_editPosXLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fY - CpSurface.Side0.fY) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPrecision,CpSurface.Side0.fY * theApp.m_fUnits, theApp.m_Units);
			m_editPosYLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fZ - CpSurface.Side0.fZ) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPrecision,CpSurface.Side0.fZ * theApp.m_fUnits, theApp.m_Units);
			m_editPosZLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fXt - CpSurface.Side0.fXt) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPrecision,CpSurface.Side0.fXt, Unit[theApp.m_Axes[theApp.m_Tank.nXtLeft].nUnitSelected]);
			m_editPosXtLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fYt - CpSurface.Side0.fYt) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPrecision,CpSurface.Side0.fYt, Unit[theApp.m_Axes[theApp.m_Tank.nYtLeft].nUnitSelected]);
			m_editPosZtLeft.SetWindowText(Buff);
		}

		if(m_CpOld.Side1.fX - CpSurface.Side1.fX) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXRight].nPrecision,CpSurface.Side1.fX * theApp.m_fUnits, theApp.m_Units);
			m_editPosXRight.SetWindowText(Buff);
		}
		if(m_CpOld.Side1.fY - CpSurface.Side1.fY) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYRight].nPrecision,CpSurface.Side1.fY * theApp.m_fUnits, theApp.m_Units);
			m_editPosYRight.SetWindowText(Buff);
		}
		if(m_CpOld.Side1.fZ - CpSurface.Side1.fZ) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZRight].nPrecision,CpSurface.Side1.fZ * theApp.m_fUnits, theApp.m_Units);
			m_editPosZRight.SetWindowText(Buff);
		}
		if(m_CpOld.Side1.fXt - CpSurface.Side1.fXt) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPrecision,CpSurface.Side1.fXt, Unit[theApp.m_Axes[theApp.m_Tank.nXtRight].nUnitSelected]);
			m_editPosXtRight.SetWindowText(Buff);
		}
		if(m_CpOld.Side1.fYt - CpSurface.Side1.fYt) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPrecision,CpSurface.Side1.fYt, Unit[theApp.m_Axes[theApp.m_Tank.nYtRight].nUnitSelected]);
			m_editPosZtRight.SetWindowText(Buff);
		}

		if(m_CpOld.Side0.fR - CpSurface.Side0.fR) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPrecision,CpSurface.Side0.fR, Unit[theApp.m_Axes[theApp.m_Tank.nRLeft].nUnitSelected]);
			m_editPosR.SetWindowText(Buff);
		}

		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXBridge].nPrecision,	(theApp.m_Axes[theApp.m_Tank.nXBridge].nPos*theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize), Unit[theApp.m_Axes[theApp.m_Tank.nXBridge].nUnitSelected]);
		m_editPosBridge.SetWindowText(Buff);

		break;
	case 1:
		if(m_CpOld.Side1.fX - CpSurface.Side1.fX) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXRight].nPrecision,CpSurface.Side1.fX * theApp.m_fUnits, theApp.m_Units);
			m_editPosXLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side1.fY - CpSurface.Side1.fY) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYRight].nPrecision,CpSurface.Side1.fY * theApp.m_fUnits, theApp.m_Units);
			m_editPosYLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side1.fZ - CpSurface.Side1.fZ) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZRight].nPrecision,CpSurface.Side1.fZ * theApp.m_fUnits, theApp.m_Units);
			m_editPosZLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side1.fXt - CpSurface.Side1.fXt) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPrecision,CpSurface.Side1.fXt, Unit[theApp.m_Axes[theApp.m_Tank.nXtRight].nUnitSelected]);
			m_editPosXtLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side1.fYt - CpSurface.Side1.fYt) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPrecision,CpSurface.Side1.fYt, Unit[theApp.m_Axes[theApp.m_Tank.nYtRight].nUnitSelected]);
			m_editPosZtLeft.SetWindowText(Buff);
		}

		if(m_CpOld.Side0.fX - CpSurface.Side0.fX) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPrecision,CpSurface.Side0.fX * theApp.m_fUnits, theApp.m_Units);
			m_editPosXRight.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fY - CpSurface.Side0.fY) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPrecision,CpSurface.Side0.fY * theApp.m_fUnits, theApp.m_Units);
			m_editPosYRight.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fZ - CpSurface.Side0.fZ) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPrecision,CpSurface.Side0.fZ * theApp.m_fUnits, theApp.m_Units);
			m_editPosZRight.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fXt - CpSurface.Side0.fXt) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPrecision,CpSurface.Side0.fXt, Unit[theApp.m_Axes[theApp.m_Tank.nXtLeft].nUnitSelected]);
			m_editPosXtRight.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fYt - CpSurface.Side0.fYt) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPrecision,CpSurface.Side0.fYt, Unit[theApp.m_Axes[theApp.m_Tank.nYtLeft].nUnitSelected]);
			m_editPosZtRight.SetWindowText(Buff);
		}

		if(m_CpOld.Side0.fR - CpSurface.Side1.fR) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPrecision,CpSurface.Side1.fR, Unit[theApp.m_Axes[theApp.m_Tank.nRLeft].nUnitSelected]);
			m_editPosR.SetWindowText(Buff);
		}

		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXBridge].nPrecision,CpSurface.Side1.fYt, Unit[theApp.m_Axes[theApp.m_Tank.nXBridge].nUnitSelected]);
		m_editPosBridge.SetWindowText(Buff);
		break;
	}

	m_CpOld = CpSurface;

	CResizablePage::OnTimer(nIDEvent);
}


void CPositionPage::OnSelchangeComboPositionMode() 
{

	theApp.m_LastSettings.nPositionDisplayMode =m_comboPositionMode.GetCurSel();
}
