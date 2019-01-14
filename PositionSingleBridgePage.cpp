// PositionSingleBridgePage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PositionSingleBridgePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPositionSingleBridgePage property page

IMPLEMENT_DYNCREATE(CPositionSingleBridgePage, CResizablePage)

CPositionSingleBridgePage::CPositionSingleBridgePage() : CResizablePage(CPositionSingleBridgePage::IDD)
{
	//{{AFX_DATA_INIT(CPositionSingleBridgePage)
	//}}AFX_DATA_INIT
	
	m_pFont = FALSE;
}

CPositionSingleBridgePage::~CPositionSingleBridgePage()
{
	SAFE_DELETE( m_pFont );

}

void CPositionSingleBridgePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPositionSingleBridgePage)
	DDX_Control(pDX, IDC_COMBO_ROTATE_MODE, m_comboRotateMode);
	DDX_Control(pDX, IDC_STATIC_RLEFT, m_staticR);
	DDX_Control(pDX, IDC_EDIT_POS_RLEFT, m_editPosR);
	DDX_Control(pDX, IDC_BUTTON_ZEROZt, m_buttonZeroZt);
	DDX_Control(pDX, IDC_BUTTON_ZEROZ, m_buttonZeroZ);
	DDX_Control(pDX, IDC_BUTTON_ZEROY, m_buttonZeroY);
	DDX_Control(pDX, IDC_BUTTON_ZEROXt, m_buttonZeroXt);
	DDX_Control(pDX, IDC_BUTTON_ZEROX, m_buttonZeroX);
	DDX_Control(pDX, IDC_COMBO_POSITION_MODE, m_comboPositionMode);
	DDX_Control(pDX, IDC_STATIC_ZTLEFT, m_staticZtLeft);
	DDX_Control(pDX, IDC_STATIC_ZLEFT, m_staticZLeft);
	DDX_Control(pDX, IDC_STATIC_YLEFT, m_staticYLeft);
	DDX_Control(pDX, IDC_STATIC_XTLEFT, m_staticXtLeft);
	DDX_Control(pDX, IDC_STATIC_XLEFT, m_staticXLeft);
	DDX_Control(pDX, IDC_EDIT_POS_ZTLEFT, m_editPosZtLeft);
	DDX_Control(pDX, IDC_EDIT_POS_ZLEFT, m_editPosZLeft);
	DDX_Control(pDX, IDC_EDIT_POS_YLEFT, m_editPosYLeft);
	DDX_Control(pDX, IDC_EDIT_POS_XTLEFT, m_editPosXtLeft);
	DDX_Control(pDX, IDC_EDIT_POS_XLEFT, m_editPosXLeft);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPositionSingleBridgePage, CPropertyPage)
	//{{AFX_MSG_MAP(CPositionSingleBridgePage)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_POSITION_MODE, OnSelchangeComboPositionMode)
	ON_BN_CLICKED(IDC_BUTTON_ZEROX, OnButtonZerox)
	ON_BN_CLICKED(IDC_BUTTON_ZEROY, OnButtonZeroy)
	ON_BN_CLICKED(IDC_BUTTON_ZEROZ, OnButtonZeroz)
	ON_BN_CLICKED(IDC_BUTTON_ZEROXt, OnBUTTONZEROXt)
	ON_BN_CLICKED(IDC_BUTTON_ZEROZt, OnBUTTONZEROZt)
	ON_CBN_SELCHANGE(IDC_COMBO_ROTATE_MODE, OnSelchangeComboRotateMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPositionSingleBridgePage message handlers

BOOL CPositionSingleBridgePage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	CString Buff;

	GetClientRect(m_rrClient);

	AddAnchor(&m_editPosXLeft);
	AddAnchor(&m_editPosYLeft);
	AddAnchor(&m_editPosZLeft);
	AddAnchor(&m_editPosXtLeft);
	AddAnchor(&m_editPosZtLeft);
	AddAnchor(&m_editPosR);
	AddAnchor(&m_staticXLeft);
	AddAnchor(&m_staticYLeft);
	AddAnchor(&m_staticZLeft);
	AddAnchor(&m_staticXtLeft);
	AddAnchor(&m_staticZtLeft);
	AddAnchor(&m_staticR);
	AddAnchor(&m_comboPositionMode);
	AddAnchor(&m_buttonZeroX);
	AddAnchor(&m_buttonZeroY);
	AddAnchor(&m_buttonZeroZ);
	AddAnchor(&m_buttonZeroXt);
	AddAnchor(&m_buttonZeroZt);
	AddAnchor(&m_comboRotateMode);


	Buff.LoadString(IDS_Encoder); m_comboPositionMode.AddString(Buff);
	Buff.LoadString(IDS_Head); m_comboPositionMode.AddString(Buff);
	Buff.LoadString(IDS_Surface); m_comboPositionMode.AddString(Buff);
	Buff.LoadString(IDS_Surface_Water); m_comboPositionMode.AddString(Buff);
	m_comboPositionMode.SetCurSel(theApp.m_LastSettings.nPositionDisplayMode);


	CFont* pFont = GetFont();
	pFont->GetLogFont(&m_LogFont);

	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&m_LogFont);
	

	if( (theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nXLeft].nHomeMode==5) ){
		m_buttonZeroX.ModifyStyle(0,WS_VISIBLE);
	}
	if( (theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nYLeft].nHomeMode==5) ){
		m_buttonZeroY.ModifyStyle(0,WS_VISIBLE);
	}
	if( (theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nZLeft].nHomeMode==5) ){
		m_buttonZeroZ.ModifyStyle(0,WS_VISIBLE);
	}
	if( (theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nXtLeft].nHomeMode==5) ){
		m_buttonZeroXt.ModifyStyle(0,WS_VISIBLE);
	}
	if( (theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nYtLeft].nHomeMode==5) ){
		m_buttonZeroZt.ModifyStyle(0,WS_VISIBLE);
	}

	
	Buff.Format(L"%s",theApp.m_Axes[theApp.m_Tank.nXtLeft].cName);
//	theApp.CharToCString(theApp.m_Axes[theApp.m_Tank.nXtLeft].cName, Buff);
	if(Buff.GetLength()>0)
		m_staticXtLeft.SetWindowText(Buff);

	Buff.Format(L"%s",theApp.m_Axes[theApp.m_Tank.nYtLeft].cName);
//	theApp.CharToCString(theApp.m_Axes[theApp.m_Tank.nYtLeft].cName, Buff);
	if(Buff.GetLength()>0)
		m_staticZtLeft.SetWindowText(Buff);


	//Only have edit dialog and static present if axis present

	if(!theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable){
		m_editPosXLeft.ModifyStyle(WS_VISIBLE,0);
		m_staticXLeft.ModifyStyle(WS_VISIBLE,0);
	}
	if(!theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable){
		m_editPosYLeft.ModifyStyle(WS_VISIBLE,0);
		m_staticYLeft.ModifyStyle(WS_VISIBLE,0);
	}
	if(!theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable){
		m_editPosZLeft.ModifyStyle(WS_VISIBLE,0);
		m_staticZLeft.ModifyStyle(WS_VISIBLE,0);
	}
	if(!theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable){
		m_editPosXtLeft.ModifyStyle(WS_VISIBLE,0);
		m_staticXtLeft.ModifyStyle(WS_VISIBLE,0);
	}
	if(!theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable){
		m_editPosZtLeft.ModifyStyle(WS_VISIBLE,0);
		m_staticZtLeft.ModifyStyle(WS_VISIBLE,0);
	}


	if(( theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable) || ( theApp.m_Axes[theApp.m_Tank.nRRight].bAvailable)) {
		m_staticR.ModifyStyle(0,WS_VISIBLE);
		m_editPosR.ModifyStyle(0,WS_VISIBLE);
	}

	Buff.LoadString(IDS_No_Rotate);			m_comboRotateMode.AddString(Buff);
	Buff.LoadString(IDS_Rotate_to_zero);	m_comboRotateMode.AddString(Buff);
	Buff.LoadString(IDS_Rotate_to_180);		m_comboRotateMode.AddString(Buff);

	m_comboRotateMode.SetCurSel(theApp.m_LastSettings.nRotateSurfaceCoordinates);

	SetTimer(1,300,NULL);

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPositionSingleBridgePage::AddAnchor(CWnd *pCtrl)
{
	CRect rr;

	pCtrl->GetWindowRect(&rr);
	ScreenToClient(&rr);

	int nID = pCtrl->GetDlgCtrlID();
	CResizablePage::AddAnchor(nID, CSize(MulDiv(rr.left,100,m_rrClient.Width()),MulDiv(rr.top,100,m_rrClient.Height())), CSize(MulDiv(rr.right,100,m_rrClient.Width()),MulDiv(rr.bottom,100,m_rrClient.Height())));	
}

void CPositionSingleBridgePage::OnSize(UINT nType, int cx, int cy) 
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
		m_editPosXLeft.SetFont(m_pFont);
		m_editPosYLeft.SetFont(m_pFont);
		m_editPosZLeft.SetFont(m_pFont);
		m_editPosXtLeft.SetFont(m_pFont);
		m_editPosZtLeft.SetFont(m_pFont);
		m_editPosR.SetFont(m_pFont);
		m_staticXLeft.SetFont(m_pFont);
		m_staticYLeft.SetFont(m_pFont);
		m_staticZLeft.SetFont(m_pFont);
		m_staticXtLeft.SetFont(m_pFont);
		m_staticZtLeft.SetFont(m_pFont);
		m_staticR.SetFont(m_pFont);
		m_comboPositionMode.SetFont(m_pFont);
		m_buttonZeroX.SetFont(m_pFont);
		m_buttonZeroY.SetFont(m_pFont);
		m_buttonZeroZ.SetFont(m_pFont);
		m_buttonZeroXt.SetFont(m_pFont);
		m_comboRotateMode.SetFont(m_pFont);
	}

}

void CPositionSingleBridgePage::OnTimer(UINT nIDEvent) 
{
	CString Buff;
	CString Unit[] = {L"mm", L"\x022", L"\x0b0"};
	CCoord CpHead,CpSurface;
	CpSurface.Zero();

	switch(theApp.m_LastSettings.nPositionDisplayMode) {
	case 0: theApp.m_Motors.GetEncoderPos(&CpSurface);
		break;
	case 1:	theApp.m_Motors.GetHeadPos(&CpSurface);
		break;
	case 2:
	case 3:
		theApp.m_Motors.GetSurfacePos(&CpSurface);
		break;
	}
	
	switch(theApp.m_nSide0Orientation) {
	case 0:
		if(m_CpOld.Side0.fX - CpSurface.Side0.fX) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPrecision,CpSurface.Side0.fX, Unit[theApp.m_Axes[theApp.m_Tank.nXLeft].nUnitSelected]);
			m_editPosXLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fY - CpSurface.Side0.fY) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPrecision,CpSurface.Side0.fY, Unit[theApp.m_Axes[theApp.m_Tank.nYLeft].nUnitSelected]);
			m_editPosYLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side0.fZ - CpSurface.Side0.fZ) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPrecision,CpSurface.Side0.fZ, Unit[theApp.m_Axes[theApp.m_Tank.nZLeft].nUnitSelected]);
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
		if(theApp.m_nUseTurntableOrRollers == 0) {
			if(m_CpOld.Side0.fR - CpSurface.Side0.fR) {
				Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPrecision,CpSurface.Side0.fR, Unit[theApp.m_Axes[theApp.m_Tank.nRLeft].nUnitSelected]);
				m_editPosR.SetWindowText(Buff);
			}
		} else {
			if(m_CpOld.Side1.fR - CpSurface.Side1.fR) {
				Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nRRight].nPrecision,CpSurface.Side1.fR, Unit[theApp.m_Axes[theApp.m_Tank.nRRight].nUnitSelected]);
				m_editPosR.SetWindowText(Buff);
			}
		}

		break;
	case 1:
		if(m_CpOld.Side1.fX - CpSurface.Side1.fX) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXRight].nPrecision,CpSurface.Side1.fX, Unit[theApp.m_Axes[theApp.m_Tank.nXRight].nUnitSelected]);
			m_editPosXLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side1.fY - CpSurface.Side1.fY) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYRight].nPrecision,CpSurface.Side1.fY, Unit[theApp.m_Axes[theApp.m_Tank.nYRight].nUnitSelected]);
			m_editPosYLeft.SetWindowText(Buff);
		}
		if(m_CpOld.Side1.fZ - CpSurface.Side1.fZ) {
			Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZRight].nPrecision,CpSurface.Side1.fZ, Unit[theApp.m_Axes[theApp.m_Tank.nZRight].nUnitSelected]);
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

		break;
	}

	m_CpOld = CpSurface;

	CResizablePage::OnTimer(nIDEvent);
}


void CPositionSingleBridgePage::OnSelchangeComboPositionMode() 
{
	theApp.m_LastSettings.nPositionDisplayMode =m_comboPositionMode.GetCurSel();
}
	


void CPositionSingleBridgePage::OnButtonZerox() 
{
	theApp.m_Axes[theApp.m_Tank.nXLeft].nPos =0;
}

void CPositionSingleBridgePage::OnButtonZeroy() 
{
	theApp.m_Axes[theApp.m_Tank.nYLeft].nPos =0;
}

void CPositionSingleBridgePage::OnButtonZeroz() 
{
	theApp.m_Axes[theApp.m_Tank.nZLeft].nPos =0;
}

void CPositionSingleBridgePage::OnBUTTONZEROXt() 
{
	theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos =0;
}

void CPositionSingleBridgePage::OnBUTTONZEROZt() 
{
	theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos =0;
}


void CPositionSingleBridgePage::OnSelchangeComboRotateMode() 
{
	theApp.m_LastSettings.nRotateSurfaceCoordinates = m_comboRotateMode.GetCurSel();
}
